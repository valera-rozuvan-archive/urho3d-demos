/**
 * A simple Urho3D example in one (big) file.
 * Copyright 2014 Peter Gebauer.
 * Released under the same permissive MIT-license as Urho3D.
 * https://raw.githubusercontent.com/urho3d/Urho3D/master/License.txt
 *
 * Why?
 * Because A first "simple" example tutorial shouldn't require additional
 * frameworks or special toolchains. This file along with Urho3D and a C++
 * compiler should do it. (you might have to change the prefix path in
 * MyApp::Setup)
 * Many (like me) want to learn themselves and just get an overview
 * without the overhead of understanding how the example is built.
 * I hope this file covers the basics and is of use to you.
 *
 * Dynamic linking on Linux with gcc 4.9.2 (minimal example):
 * (change "/usr/local" to where you have Urho3D installed)
 *   > g++ -I/usr/local/include/Urho3D -I/usr/local/include/Urho3D/ThirdParty \
 *   >    -o urho3d_simple_example.bin urho3d_simple_example.cpp \
 *   >    -lUrho3D -ldl -lpthread -lGL
 *
 * Static linking on Linux with gcc 4.9.2 (minimal example):
 * (change "/usr/local" to where you have Urho3D installed)
 *   > g++ -I/usr/local/include/Urho3D -I/usr/local/include/Urho3D/ThirdParty \
 *   >    -o urho3d_simple_example.bin urho3d_simple_example.cpp \
 *   >    -rdynamic /usr/local/lib64/Urho3D/libUrho3D.a -ldl -lpthread -lGL
 *
 * On my system I also add the following for some system specifics:
 * > -ffast-math -m64
 *
 * There's a few defines, you'll have to pick em as you see fit. Here are mine:
 * > -DGLEW_NO_GLU -DGLEW_STATIC -DHAVE_STDINT_H -DUNIX -DURHO3D_LOGGING \
 * >   -DURHO3D_OPENGL-DURHO3D_SSE -DURHO3D_STATIC_DEFINE -DURHO3D_URHO2D
 *
 * Debugging flags:
 * > -g -DDEBUG -D_DEBUG
 *
 * I recommend http://urho3d.github.io/documentation/HEAD/_using_library.html
 * and the pkg-config's it mentions. 'pkg-config --cflags --libs Urho3D' will
 * show you everything you need to know.
 *
 * If you'd like to add your working test case to the list above, let me know.
 */

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

// Thanks to lexx from Urho3D forums
#ifdef _MSC_VER
#include <cstdio>
#define snprintf _snprintf
#endif

using namespace Urho3D;

/**
 * Using the convenient Application API we don't have
 * to worry about initializing the engine or writing a main.
 * You can probably mess around with initializing the engine
 * and running a main manually, but this is convenient and portable.
 */
class MyApp : public Application
{
public:

    int framecount_;
    float time_;

    // SharedPtr<Text> text_;

    SharedPtr<Scene> scene_;
    SharedPtr<Node> boxNode_;
    SharedPtr<Node> boxNode_1;
    SharedPtr<Node> boxNode_2;
    SharedPtr<Node> boxNode_3;
    SharedPtr<Node> boxNode_4;
    SharedPtr<Node> boxNode_5;
    SharedPtr<Node> boxNode_6;

    /**
     * This happens before the engine has been initialized
     * so it's usually minimal code setting defaults for
     * whatever instance variables you have.
     * You can also do this in the Setup method.
     */
    MyApp(Context * context) : Application(context)
    {
        framecount_ = 0;
        time_ = 0.0f;
    }

    /**
     * This method is called _before_ the engine has been initialized.
     * Thusly, we can setup the engine parameters before anything else
     * of engine importance happens (such as windows, search paths,
     * resolution and other things that might be user configurable).
     */
    virtual void Setup()
    {
        // These parameters should be self-explanatory.
        // See http://urho3d.github.io/documentation/1.32/_main_loop.html
        // for a more complete list.
        engineParameters_["FullScreen"]  = false;
        engineParameters_["WindowWidth"]  = 1280;
        engineParameters_["WindowHeight"]  = 720;
        // Override the resource prefix path to use. "If not specified then the
        // default prefix path is set to URHO3D_PREFIX_PATH environment
        // variable (if defined) or executable path."
        // By default mine was in /usr/local/share, change as needed.
        // Remember to use a TRAILING SLASH to your path! (for unknown reason)
        engineParameters_["ResourcePrefixPath"]  = "/home/valera/projects/game-d/02_HelloWorld/";

    }

    /**
     * This method is called _after_ the engine has been initialized.
     * This is where you set up your actual content, such as scenes,
     * models, controls and what not. Basically, anything that needs
     * the engine initialized and ready goes in here.
     */
    virtual void Start()
    {
        // First up, we can't be grabbing nobodys mouse like that.
        // These _should_ be available as engine parameters, methinks.
        GetSubsystem<Input>()->SetMouseVisible(true);
        GetSubsystem<Input>()->SetMouseGrabbed(false);

        // We will be needing to load resources.
        // All the resources used in this example comes with Urho3D.
        // If the engine can't find them, check the ResourcePrefixPath.
        ResourceCache * cache = GetSubsystem<ResourceCache>();

        // Let's use the default style that comes with Urho3D.
        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

        /*
        // Let's create some text to display.
        text_ = new Text(context_);
        // Text will be updated later in the E_UPDATE handler. Keep readin'.
        text_->SetText("Wait a little while to see FPS!");
        // If the engine cannot find the font, it comes with Urho3D.
        // Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
        // change the engine parameter "ResourcePrefixPath" in the Setup method.
        text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),
                         30);
        text_->SetColor(Color(1.0f, 1.0f, 0.0f)); // Yellah!
        text_->SetHorizontalAlignment(HA_CENTER);
        text_->SetVerticalAlignment(VA_CENTER);
        GetSubsystem<UI>()->GetRoot()->AddChild(text_);
        */
/*
        // Add a button, just as an interactive UI sample.
        Button * button = new Button(context_);
        // Note, must be part of the UI system before SetSize calls!
        GetSubsystem<UI>()->GetRoot()->AddChild(button);
        button->SetName("Button Quit");
        button->SetStyle("Button");
        button->SetSize(32, 32);
        button->SetPosition(16, 16);
*/
        // Let's setup a scene to render.
        scene_ = new Scene(context_);
        // Let the scene have an Octree component!
        scene_->CreateComponent<Octree>();
        // Let's add an additional scene component for fun.
        scene_->CreateComponent<DebugRenderer>();

/*
        // Let's put some sky in there.
        // Again, if the engine can't find these resources you need to check
        // the "ResourcePrefixPath". These files come with Urho3D.
        Node * skyNode = scene_->CreateChild("Sky");
        skyNode->SetScale(500.0f); // The scale actually does not matter
        Skybox * skybox = skyNode->CreateComponent<Skybox>();
        skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));
*/
        // Let's put a box in there.
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

        // We need a camera from which the viewport can render.
        Node * cameraNode = scene_->CreateChild("Camera");
        Camera * camera = cameraNode->CreateComponent<Camera>();
        camera->SetFarClip(256.0f);

        // As per usual, let there be light. Put it in the camera node.
        Light * light = cameraNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_POINT);
        light->SetRange(22.0f);
        light->SetBrightness(1.0);
        light->SetSpecularIntensity(10);
        light->SetColor(Color(1.0, 1.0, 1.0, 1.0));

        // Now we setup the viewport. Ofcourse, you can have more than one!
        Renderer * renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);

        // We subscribe to the events we'd like to handle.
        // In this example we will be showing what most of them do,
        // but in reality you would only subscribe to the events
        // you really need to handle.
        // These are sort of subscribed in the order in which the engine
        // would send the events. Read each handler method's comment for
        // details.
        SubscribeToEvent(E_BEGINFRAME,
                         URHO3D_HANDLER(MyApp, HandleBeginFrame));
        SubscribeToEvent(E_KEYDOWN,
                         URHO3D_HANDLER(MyApp, HandleKeyDown));
        SubscribeToEvent(E_UIMOUSECLICK,
                         URHO3D_HANDLER(MyApp, HandleControlClicked));
        SubscribeToEvent(E_UPDATE,
                         URHO3D_HANDLER(MyApp, HandleUpdate));
        SubscribeToEvent(E_POSTUPDATE,
                         URHO3D_HANDLER(MyApp, HandlePostUpdate));
        SubscribeToEvent(E_RENDERUPDATE,
                         URHO3D_HANDLER(MyApp, HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE,
                         URHO3D_HANDLER(MyApp, HandlePostRenderUpdate));
        SubscribeToEvent(E_ENDFRAME,
                         URHO3D_HANDLER(MyApp, HandleEndFrame));
    }

    /**
     * Good place to get rid of any system resources that requires the
     * engine still initialized. You could do the rest in the destructor,
     * but there's no need, this method will get called when the engine stops,
     * for whatever reason (short of a segfault).
     */
    virtual void Stop()
    {
    }

    /**
     * Every frame's life must begin somewhere. Here it is.
     */
    void HandleBeginFrame(StringHash eventType, VariantMap & eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

    /**
     * Input from keyboard is handled here. I'm assuming that Input, if
     * available, will be handled before E_UPDATE.
     */
    void HandleKeyDown(StringHash eventType, VariantMap & eventData)
    {
        using namespace KeyDown;
        int key = eventData[P_KEY].GetInt();
        // T'is a good default key for quit things.
        if (key == KEY_ESC)
            engine_->Exit();
    }

    /**
     * You can get these events from when ever the user interacts with the UI.
     */
    void HandleControlClicked(StringHash eventType, VariantMap& eventData)
    {
        // Since we only have one UI item (the button) we know it's been clicked.
        // But, if you want to grab the clicked UI control, do this:
        // UIElement* clicked = static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());
        engine_->Exit();
    }

    /**
     * Your non-rendering logic should be handled here.
     * This could be moving objects, checking collisions and reaction, etc.
     */
    void HandleUpdate(StringHash eventType, VariantMap & eventData)
    {
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
        framecount_++;
        time_ += timeStep;

        /*
        if (time_ >= 2.0) {
            // Every two seconds we update the FPS text and reset.
            char tmp[128];
            // Sorry for the C, I don't like C++ and in particular streams.
            snprintf(tmp, 128, "%d frames in %f seconds = %.2f fps", framecount_, time_, (float)framecount_ / time_);
            String s(tmp);
            text_->SetText(s);
            URHO3D_LOGINFO(s);
            framecount_ = 0;
            time_ = 0.0f;
        }
        */

        // Rotate the box thingy.
        // A much nicer way of doing this would be with a LogicComponent.
        // With LogicComponents it is easy to control things like movement
        // and animation from some IDE, console or just in game.
        // Alas, it is out of the scope for our simple example.
        boxNode_->Rotate(Quaternion(8 * timeStep, 16 * timeStep, 0));
        boxNode_1->Rotate(Quaternion(12 * timeStep, 9 * timeStep, 0));
        boxNode_2->Rotate(Quaternion(20 * timeStep, 3 * timeStep, 0));
        boxNode_3->Rotate(Quaternion(17 * timeStep, -11 * timeStep, 0));
        boxNode_4->Rotate(Quaternion(-5 * timeStep, 7 * timeStep, 0));
        boxNode_5->Rotate(Quaternion(-10 * timeStep, -2 * timeStep, 0));
        boxNode_6->Rotate(Quaternion(19 * timeStep, -16 * timeStep, 0));
    }

    /**
     * Anything in the non-rendering logic that requires a second pass,
     * it might be well suited to be handled here.
     */
    void HandlePostUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

    /**
     * If you have any details you want to change before the viewport is
     * rendered, try putting it here.
     * See http://urho3d.github.io/documentation/1.32/_rendering.html
     * for details on how the rendering pipeline is setup.
     */
    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

    /**
     * After everything is rendered, there might still be things you wish
     * to add to the rendering. At this point you cannot modify the scene,
     * only post rendering is allowed. Good for adding things like debug
     * artifacts on screen or brush up lighting, etc.
     */
    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We could draw some debuggy looking thing for the octree.
        // scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
    }

    /**
     * All good things must come to an end.
     */
    void HandleEndFrame(StringHash eventType, VariantMap & eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

};


/**
 * This macro is expaneded to (roughly, depending on OS) this:
 *
 * > int RunApplication()
 * > {
 * >     Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
 * >     Urho3D::SharedPtr<className> application(new className(context));
 * >     return application->Run();
 * > }
 * >
 * > int main(int argc, char** argv)
 * > {
 * >     Urho3D::ParseArguments(argc, argv);
 * >     return function;
 * > }
 */
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
