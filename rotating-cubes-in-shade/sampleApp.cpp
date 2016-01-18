#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>

#ifdef _MSC_VER
#include <cstdio>
#define snprintf _snprintf
#endif

using namespace Urho3D;

class MyApp : public Application
{
public:
    SharedPtr<Scene> scene_;
    SharedPtr<Node> boxNode_;
    SharedPtr<Node> boxNode_1;
    SharedPtr<Node> boxNode_2;
    SharedPtr<Node> boxNode_3;
    SharedPtr<Node> boxNode_4;
    SharedPtr<Node> boxNode_5;
    SharedPtr<Node> boxNode_6;

    MyApp(Context * context) : Application(context)
    {
    }

    virtual void Setup()
    {
        engineParameters_["FullScreen"]  = false;
        engineParameters_["WindowWidth"]  = 1280;
        engineParameters_["WindowHeight"]  = 720;
        engineParameters_["ResourcePrefixPath"]  = "";

    }

    virtual void Start()
    {
        GetSubsystem<Input>()->SetMouseVisible(true);
        GetSubsystem<Input>()->SetMouseGrabbed(false);

        ResourceCache * cache = GetSubsystem<ResourceCache>();

        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

        scene_ = new Scene(context_);
        scene_->CreateComponent<Octree>();
        scene_->CreateComponent<DebugRenderer>();

        boxNode_ = scene_->CreateChild("Box");
        boxNode_->SetPosition(Vector3(0, 0, 32));
        boxNode_->SetScale(20.0f);
        StaticModel * boxObject = boxNode_->CreateComponent<StaticModel>();
        boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_1 = scene_->CreateChild("Box");
        boxNode_1->SetPosition(Vector3(0, 1, 5));
        boxNode_1->SetScale(0.85f);
        StaticModel * boxObject1 = boxNode_1->CreateComponent<StaticModel>();
        boxObject1->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject1->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_3 = scene_->CreateChild("Box");
        boxNode_3->SetPosition(Vector3(2, 1, 5));
        boxNode_3->SetScale(0.70f);
        StaticModel * boxObject3 = boxNode_3->CreateComponent<StaticModel>();
        boxObject3->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject3->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_4 = scene_->CreateChild("Box");
        boxNode_4->SetPosition(Vector3(2, -1, 5));
        boxNode_4->SetScale(0.25f);
        StaticModel * boxObject4 = boxNode_4->CreateComponent<StaticModel>();
        boxObject4->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject4->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_5 = scene_->CreateChild("Box");
        boxNode_5->SetPosition(Vector3(-2, 1, 5));
        boxNode_5->SetScale(1.0f);
        StaticModel * boxObject5 = boxNode_5->CreateComponent<StaticModel>();
        boxObject5->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject5->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_6 = scene_->CreateChild("Box");
        boxNode_6->SetPosition(Vector3(-2, -1, 5));
        boxNode_6->SetScale(0.55f);
        StaticModel * boxObject6 = boxNode_6->CreateComponent<StaticModel>();
        boxObject6->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject6->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        boxNode_2 = scene_->CreateChild("Box");
        boxNode_2->SetPosition(Vector3(0, -1, 5));
        boxNode_2->SetScale(0.40f);
        StaticModel * boxObject2 = boxNode_2->CreateComponent<StaticModel>();
        boxObject2->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject2->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

        Node * cameraNode = scene_->CreateChild("Camera");
        Camera * camera = cameraNode->CreateComponent<Camera>();
        camera->SetFarClip(256.0f);

        Light * light = cameraNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_POINT);
        light->SetRange(22.0f);
        light->SetBrightness(1.0);
        light->SetSpecularIntensity(10);
        light->SetColor(Color(1.0, 1.0, 1.0, 1.0));

        Renderer * renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);

        SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(MyApp, HandleBeginFrame));
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKeyDown));
        SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(MyApp, HandleControlClicked));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
        SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(MyApp, HandlePostUpdate));
        SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(MyApp, HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(MyApp, HandlePostRenderUpdate));
        SubscribeToEvent(E_ENDFRAME, URHO3D_HANDLER(MyApp, HandleEndFrame));
    }

    virtual void Stop()
    {
    }

    void HandleBeginFrame(StringHash eventType, VariantMap & eventData)
    {
    }

    void HandleKeyDown(StringHash eventType, VariantMap & eventData)
    {
        using namespace KeyDown;
        int key = eventData[P_KEY].GetInt();

        if (key == KEY_ESC)
            engine_->Exit();
    }

    void HandleControlClicked(StringHash eventType, VariantMap& eventData)
    {
        engine_->Exit();
    }

    void HandleUpdate(StringHash eventType, VariantMap & eventData)
    {
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

        boxNode_->Rotate(Quaternion(8 * timeStep, 16 * timeStep, 0));
        boxNode_1->Rotate(Quaternion(12 * timeStep, 9 * timeStep, 0));
        boxNode_2->Rotate(Quaternion(20 * timeStep, 3 * timeStep, 0));
        boxNode_3->Rotate(Quaternion(17 * timeStep, -11 * timeStep, 0));
        boxNode_4->Rotate(Quaternion(-5 * timeStep, 7 * timeStep, 0));
        boxNode_5->Rotate(Quaternion(-10 * timeStep, -2 * timeStep, 0));
        boxNode_6->Rotate(Quaternion(19 * timeStep, -16 * timeStep, 0));
    }

    void HandlePostUpdate(StringHash eventType, VariantMap & eventData)
    {
    }

    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
    }

    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
    }

    void HandleEndFrame(StringHash eventType, VariantMap & eventData)
    {
    }

};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
