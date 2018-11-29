#include <Urho3D/Urho3DAll.h>

#include <Urhox.h>
#include <ImGui/imgui.h>
#include "Spark/include/SPARK.h"
#include "../Spark/Spark.h"

using namespace Urho3D;



///---------------------------------------------------------------------------------------------
/// Custom logic component for moving a node around a circle with sin/cos variations.
///---------------------------------------------------------------------------------------------
class FxMover : public LogicComponent
{
    URHO3D_OBJECT(FxMover, LogicComponent)

public:

    /// Construct.
    FxMover(Context* context) :
      LogicComponent(context),
        speed_(0.0f),
        radius_(0.0f),
        elapsedTime_(0.0f)
    {
        // Only the scene update event is needed: unsubscribe from the rest for optimization
        SetUpdateEventMask(USE_UPDATE);
    }

    /// Set motion parameters: forward movement speed, rotation speed
    void SetParameters(float speed, float radius)
    {
        speed_ = speed;
        radius_ = radius;
    }

    /// Handle scene update. Called by LogicComponent base class.
    void Update(float timeStep)
    {
        Vector3 pos = node_->GetPosition();
        pos.x_ = sin(elapsedTime_ * speed_) * radius_;
        pos.y_ = 1.2f + sin(elapsedTime_ * speed_ * 10.0f) * 1.0f;
        pos.z_ = cos(elapsedTime_ * speed_) * radius_;

        node_->SetPosition(pos);

        elapsedTime_ += timeStep;
    }

private:
    /// Moving speed
    float speed_;
    /// Circle radius
    float radius_;
    /// Elasped time
    float elapsedTime_;
};



/// Spark Particles scene example.
/// This sample demonstrates:
///     - How to build a new Spark Particles effect from scratch.
///     - Using Spark Particle files to load particles effects.
///     - Creating a 3D Scene with Spark Particles.
class SparkParticles : public Application
{

private:

    SPK::Ref<SPK::System> effectExplosion_;
    SPK::Ref<SPK::System> effectFountain_;
    Node* magicBallNode_;

    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    float yaw_;
    float pitch_;

public:
    /// Construct.
    SparkParticles(Context* context) :
        Application(context)
    {
        context->RegisterFactory<FxMover>();
    }

    void Setup() override
    {
        engineParameters_[EP_WINDOW_TITLE] = "Sample";
        engineParameters_[EP_FULL_SCREEN] = false;
        engineParameters_[EP_WINDOW_WIDTH] = 1280;
        engineParameters_[EP_WINDOW_HEIGHT] = 720;
        engineParameters_[EP_WINDOW_RESIZABLE] = true;
    }

    /// Setup after engine initialization and before running the main loop.
    void Start() override
    {
        // Build manually a Spark Effect explosion from scratch.
        CreateExplosionEffect();

        // Create the scene content
        CreateScene();

        // Create the UI content
        CreateInstructions();

        // Hook up to the frame update events
        SubscribeToEvents();
    }

    /// Spawn an explosion effect at position.
    void AddExplosion(Vector3 pos)
    {

    }

private:
    /// Construct the scene content.
    void CreateScene()
    {
        ResourceCache* cache = GetSubsystem<ResourceCache>();

        scene_ = new Scene(context_);
        scene_->CreateComponent<Octree>();

        // Create static scene content. First create a zone for ambient lighting and fog control
        Node* zoneNode = scene_->CreateChild("Zone");
        Zone* zone = zoneNode->CreateComponent<Zone>();
        zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
        zone->SetFogColor(Color(0.2f, 0.2f, 0.4f));
        zone->SetFogStart(100.0f);
        zone->SetFogEnd(300.0f);
        zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));


        // There is 2 ways of using Spark Particles.

        // 1. Build manually in memory a spark effect using code.

        // Create Spark effect from scratch
        CreateFountainEffect();

        // Once effect is builded, assign it to a SparkParticleEffect and add it to the cache.
        SparkParticleEffect* effect = new SparkParticleEffect(context_);
        effect->SetSystem(effectFountain_);
        effect->SetName("MyNewSparkParticleEffectResource");
        cache->AddManualResource(effect); // ! important for clones

        // Now, we can use this effect inside a SparkParticle component.
        Node* node1 = scene_->CreateChild("Spark1");
        node1->SetPosition(Vector3(10.0f, 0.0f, 0.0f));
        SparkParticle * sparkComponent1 = node1->CreateComponent<SparkParticle>();
        sparkComponent1->SetEffect(effect);


        // 2. Load a spark effect file and use a SparkParticle component.

        // Create a new child scene node and a create a SparkParticle component into it.
        // Set effect by loading a SparkParticleEffect resource.
        // Note1: SparkParticleEffect can load .xml or .spk files
        // Note2: Spark effect .xml and .spk files can be created by exporting a manually builded effect or using a spark particle editor.


        // Create some particles nodes.

        Node* nodeEffect1 = scene_->CreateChild("Fire");
        nodeEffect1->SetPosition(Vector3(-10.0f, 0.0f, 0.0f));
        SparkParticle * spk1 = nodeEffect1->CreateComponent<SparkParticle>();
        spk1->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/FireAtlas.xml"));

        Node* nodeEffect2 = scene_->CreateChild("Bubbles");
        nodeEffect2->SetPosition(Vector3(-5.0f, 5.0f, 0.0f));
        SparkParticle * spk2 = nodeEffect2->CreateComponent<SparkParticle>();
        spk2->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/Bubbles.xml"));

        Node* nodeEffect3 = scene_->CreateChild("Vortex");
        nodeEffect3->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        SparkParticle * spk3 = nodeEffect3->CreateComponent<SparkParticle>();
        spk3->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/Vortex.xml"));

        Node* nodeEffect4 = scene_->CreateChild("Spark");
        nodeEffect4->SetPosition(Vector3(5.0f, 2.0f, 0.0f));
        SparkParticle * spk4 = nodeEffect4->CreateComponent<SparkParticle>();
        spk4->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/Spark1.xml"));

        // Create a node moving around a circle
        // Attach to it a ball model, a light and particles effect.
        {
            magicBallNode_ = scene_->CreateChild("MagicBall");
            magicBallNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

            FxMover* mover = magicBallNode_->CreateComponent<FxMover>();
            mover->SetParameters(2.0f, 15.0f);

            StaticModel* ball = magicBallNode_->CreateComponent<StaticModel>();
            ball->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
            ball->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

            Light* lightBall = magicBallNode_->CreateComponent<Light>();
            lightBall->SetLightType(LIGHT_POINT);
            lightBall->SetRange(3.0f);
            lightBall->SetBrightness(2.0f);
            lightBall->SetColor(Color(0.9f, 0.6f, 0.1f));

            SparkParticle * spkMagicBall = magicBallNode_->CreateComponent<SparkParticle>();
            spkMagicBall->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/MagicBall.xml"));
        }


        // Create some burning mushrooms.
        const unsigned NUM_OBJECTS = 18;
        for (unsigned i = 0; i < NUM_OBJECTS; ++i)
        {
            float posX = Cos(i * 20.0f) * 20.0f;
            float posY = Sin(i * 20.0f) * 20.0f;

            Node* mushroomNode = scene_->CreateChild("Mushroom");
            mushroomNode->SetPosition(Vector3(posX, 0.0f, posY));
            mushroomNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
            mushroomNode->SetScale(0.5f);

            StaticModel* mushroomObject = mushroomNode->CreateComponent<StaticModel>();
            mushroomObject->SetModel(cache->GetResource<Model>("Models/Mushroom.mdl"));
            mushroomObject->SetMaterial(cache->GetResource<Material>("Materials/Mushroom.xml"));

            SparkParticle* spk = mushroomNode->CreateComponent<SparkParticle>();
            spk->SetEffect(cache->GetResource<SparkParticleEffect>("Spark/Effects/Fire1.xml"));
        }


        // Create a plane.
        Node* planeNode = scene_->CreateChild("Plane");
        planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
        StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
        planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
        planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

        // Create a directional light.
        Node* lightNode = scene_->CreateChild("DirectionalLight");
        lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
        Light* light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);

        // Create a camera.
        cameraNode_ = scene_->CreateChild("Camera");
        cameraNode_->CreateComponent<Camera>();
        cameraNode_->SetPosition(Vector3(0.0f, 5.0f, -20.0f));

        // Set up a viewport
        Renderer* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);
    }

    /// Construct an instruction text to the UI.
    void CreateInstructions()
    {
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        UI* ui = GetSubsystem<UI>();

        // Construct new Text object, set string to display and font to use
        Text* instructionText = ui->GetRoot()->CreateChild<Text>();
        instructionText->SetText("Use WASD keys and mouse/touch to move\nPress Space bar to create explosions");
        instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

        // Position the text relative to the screen center
        instructionText->SetHorizontalAlignment(HA_CENTER);
        instructionText->SetVerticalAlignment(VA_CENTER);
        instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
    }

    /// Read input and moves the camera.
    void MoveCamera(float timeStep)
    {
        // Do not move if the UI has a focused element (the console)
        if (GetSubsystem<UI>()->GetFocusElement())
            return;

        Input* input = GetSubsystem<Input>();

        // Movement speed as world units per second
        const float MOVE_SPEED = 20.0f;
        // Mouse sensitivity as degrees per pixel
        const float MOUSE_SENSITIVITY = 0.1f;

        // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
        IntVector2 mouseMove = input->GetMouseMove();
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
        pitch_ = Clamp(pitch_, -90.0f, 90.0f);

        // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
        cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

        // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
        // Use the Translate() function (default local space) to move relative to the node's orientation.
        if (input->GetKeyDown(KEY_W))
            cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
        if (input->GetKeyDown(KEY_S))
            cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
        if (input->GetKeyDown(KEY_A))
            cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
        if (input->GetKeyDown(KEY_D))
            cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);


        // Add an explosion effect at random position when press space
        if (input->GetKeyPress(KEY_SPACE))
        {
            AddExplosion(Vector3(Random(40.0f) - 20.0f, 3.0f, Random(40.0f) - 20.0f));
        }
    }

    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents()
    {
        // Subscribe HandleUpdate() function for processing update events
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(SparkParticles, HandleUpdate));

    }

    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData)
    {
        using namespace Update;

        // Take the frame time step, which is stored as a float
        float timeStep = eventData[P_TIMESTEP].GetFloat();

        // Move the camera, scale movement with time step
        MoveCamera(timeStep);
    }

    /// Build an explosion effect.
    void CreateExplosionEffect()
    {

    }

    /// Build a fountain effect.
    void CreateFountainEffect()
    {

    }
};


URHO3D_DEFINE_APPLICATION_MAIN(SparkParticles)









#if 0
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

#endif


