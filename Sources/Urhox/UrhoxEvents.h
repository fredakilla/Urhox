#pragma once

namespace Urho3D
{

/// Engine finished initialization, but Application::Start() was not claled yet.
URHO3D_EVENT(E_ENGINEINITIALIZED, EngineInitialized)
{
}

/// Application started, but first frame was not executed yet.
URHO3D_EVENT(E_APPLICATIONSTARTED, ApplicationStarted)
{
}

}

