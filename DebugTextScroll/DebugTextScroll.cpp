#include "Asteroids/Util/DebugTextScroll.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/IO/IOEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>

using namespace Urho3D;

namespace Asteroids {

// ----------------------------------------------------------------------------
DebugTextScroll::DebugTextScroll(Context* context) :
    Object(context),
    timeoutSetting_(5)
{
    SetTextCount(20);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(DebugTextScroll, HandleUpdate));
    SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(DebugTextScroll, HandleLogMessage));
}

// ----------------------------------------------------------------------------
void DebugTextScroll::SetTextCount(unsigned count)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    while(items_.Size() < count)
    {
        TextItem item;
        item.text_ = ui->GetRoot()->CreateChild<Text>();
        item.text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 9);
        item.text_->SetTextAlignment(HA_LEFT);
        item.text_->SetHorizontalAlignment(HA_LEFT);
        item.text_->SetVerticalAlignment(VA_TOP);
        item.timeout_ = 0;
        items_.Push(item);
    }

    while(items_.Size() > count)
    {
        items_.Pop();
    }

    insertIt_ = items_.Begin();
}

// ----------------------------------------------------------------------------
void DebugTextScroll::SetTimeout(float seconds)
{
    timeoutSetting_ = seconds;
}

// ----------------------------------------------------------------------------
void DebugTextScroll::Print(const String& str, const Color& color)
{
    if(insertIt_.ptr_ == NULL)
        return;
    UI* ui = GetSubsystem<UI>();
    if (ui == nullptr)
        return;
    UIElement* root = ui->GetRoot();
    if (root == nullptr)
        return;

    insertIt_->text_->SetText(str);
    insertIt_->text_->SetOpacity(1);
    insertIt_->text_->SetColor(color);
    insertIt_->text_->SetPosition(0, root->GetHeight() - 40);
    insertIt_->timeout_ = timeoutSetting_;

    if(++insertIt_ == items_.End())
        insertIt_ = items_.Begin();

    for(Vector<TextItem>::Iterator it = items_.Begin(); it != items_.End(); ++it)
    {
        IntVector2 pos = it->text_->GetPosition();
        it->text_->SetPosition(pos.x_, pos.y_ - it->text_->GetHeight() - 2);
    }
}

// ----------------------------------------------------------------------------
void DebugTextScroll::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    for(Vector<TextItem>::Iterator it = items_.Begin(); it != items_.End(); ++it)
    {
        if((it->timeout_ -= timeStep) < 0)
        {
            float opacity = it->timeout_ + 1;
            if(opacity > 0)
                it->text_->SetOpacity(opacity);
            else
                it->text_->SetText("");
        }
    }
}

// ----------------------------------------------------------------------------
void DebugTextScroll::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace LogMessage;
    int level = eventData[P_LEVEL].GetInt();

    Color color = Color::WHITE;
    if(level == LOG_WARNING)
        color = Color(1, 0.5, 0.1);
    if(level == LOG_ERROR)
        color = Color(1, 0.1, 0.1);
    Print(eventData[P_MESSAGE].GetString(), color);
}

}
