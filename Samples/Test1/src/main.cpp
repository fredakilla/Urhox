#include <Urho3D/Urho3DAll.h>

#include <Urhox/Urhox.h>
#include <ImGui/imgui.h>

using namespace Urho3D;


/// Custom logic component for rotating a scene node.
class Rotator : public LogicComponent
{
    URHO3D_OBJECT(Rotator, LogicComponent)

public:

    /// Construct.
    Rotator(Context* context) :
        LogicComponent(context),
        rotationSpeed_(Vector3::ZERO)
    {
        // Only the scene update event is needed: unsubscribe from the rest for optimization
        SetUpdateEventMask(USE_UPDATE);

        URHO3D_ATTRIBUTE("Rotation speed", Vector3, rotationSpeed_, Vector3::ZERO, AM_DEFAULT);
    }

    /// Set rotation speed about the Euler axes. Will be scaled with scene update time step.
    void SetRotationSpeed(const Vector3& speed)
    {
        rotationSpeed_ = speed;
    }

    /// Handle scene update. Called by LogicComponent base class.
    virtual void Update(float timeStep)
    {
        // Components have their scene node as a member variable for convenient access. Rotate the scene node now: construct a
        // rotation quaternion from Euler angles, scale rotation speed with the scene update time step
        node_->Rotate(Quaternion(rotationSpeed_.x_ * timeStep, rotationSpeed_.y_ * timeStep, rotationSpeed_.z_ * timeStep));
    }

    /// Return rotation speed.
    const Vector3& GetRotationSpeed() const
    {
        return rotationSpeed_;
    }

private:
    Vector3 rotationSpeed_;
};





class MyApp : public Application
{
    SharedPtr<Scene> _scene;

public:

    MyApp(Context* context) :
        Application(context)
    {
        context->RegisterFactory<Rotator>();
    }

    void Setup() override
    {
        engineParameters_[EP_WINDOW_TITLE] = "Sample";
        engineParameters_[EP_FULL_SCREEN] = false;
        engineParameters_[EP_WINDOW_WIDTH] = 1280;
        engineParameters_[EP_WINDOW_HEIGHT] = 720;
        engineParameters_[EP_WINDOW_RESIZABLE] = true;
    }

    void Stop() override
    {
        // Perform optional cleanup after main loop has terminated
        engine_->DumpResources(true);
    }

    void Start() override
    {
        context_->RegisterSubsystem(new SystemUI(context_));
        context_->GetSubsystem<SystemUI>()->Start();

        context_->RegisterSubsystem(new DebugHud(context_));
        context_->GetSubsystem<DebugHud>()->ToggleAll();

        ui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ui::GetIO().BackendFlags |= ImGuiBackendFlags_HasMouseCursors;


        // Seems like the mouse must be in cursor mode before creating the UI or it won't work.
        GetSubsystem<Input>()->SetMouseVisible(true);
        GetSubsystem<Input>()->SetMouseGrabbed(false);


        ResourceCache* cache = GetSubsystem<ResourceCache>();

        _scene = new Scene(context_);
        _scene->CreateComponent<Octree>();
        //_scene->CreateComponent<DebugRenderer>();

        Node* node = _scene->CreateChild("Box");
        node->SetPosition(Vector3(0,0,0));
        StaticModel* staticModel = node->CreateComponent<StaticModel>();
        staticModel->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        staticModel->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        Rotator* rotator = node->CreateComponent<Rotator>();
        rotator->SetRotationSpeed(Vector3(10.0f, 400.0f, 30.0f));

        Node* lightNode = _scene->CreateChild("DirectionalLight");
        lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized
        auto* light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);

        Node* nodeCamera = _scene->CreateChild("Camera");
        nodeCamera->SetPosition(Vector3(0,1,-5));
        Camera* camera = nodeCamera->CreateComponent<Camera>();
        camera->SetFarClip(2000);

        Renderer* renderer = GetSubsystem<Renderer>();
        Viewport* viewport = new Viewport(context_, _scene, camera);
        renderer->SetViewport(0, viewport);

        // Called after engine initialization. Setup application & subscribe to events here
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
    }    

    void HandleUpdate(StringHash eventType, VariantMap& eventData)
    {
        // Show ImGui test window
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();

        // Create some ImGui controls to manage cube rotation
        static float axis[] = { 0.0f, 10.0f, 0.0f };
        ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Cube rotation speed");
        ImGui::SliderFloat3("Axis", axis, -200.0f, 200.0f);
        ImGui::End();

        Rotator* rotator = _scene->GetChild("Box")->GetComponent<Rotator>();
        rotator->SetRotationSpeed(Vector3(axis));

    }

    void HandleKeyDown(StringHash eventType, VariantMap& eventData)
    {
        using namespace KeyDown;
        // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
        int key = eventData[P_KEY].GetInt();
        if (key == KEY_ESCAPE)
            engine_->Exit();
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)



