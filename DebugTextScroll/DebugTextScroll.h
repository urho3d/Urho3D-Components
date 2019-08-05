#pragma once

#include <Urho3D/Core/Object.h>

#if defined(DEBUG)
#   include <Urho3D/IO/Log.h>
#   define LOG_SCROLL(msg) do {                                \
            DebugTextScroll* debug_##__LINE__ =                \
                    GetSubsystem<DebugTextScroll>();           \
            if(debug_##__LINE__) debug_##__LINE__->Print(msg); \
            URHO3D_LOGDEBUG(msg);                              \
    } while(0)
#else
#   define LOG_SCROLL(msg)
#endif

namespace Urho3D {
class Text;

class URHO3D_API DebugTextScroll : public Urho3D::Object
{
    URHO3D_OBJECT(DebugTextScroll, Urho3D::Object);

public:
    DebugTextScroll(Urho3D::Context* context);

    /*!
     * @brief Sets the maximum number of lines to print to the screen.
     * Defaults to 20.
     */
    void SetTextCount(unsigned count);

    /*!
     * @brief Sets how long a message should remain on the screen before rading
     * out. Defaults to 5 seconds.
     */
    void SetTimeout(float seconds);

    /*!
     * @brief Adds a message to print to the screen.
     */
    void Print(const Urho3D::String& str, const Urho3D::Color& color=Urho3D::Color::WHITE);

private:
    void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleLogMessage(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    struct TextItem
    {
        Urho3D::SharedPtr<Urho3D::Text> text_;
        float timeout_;
    };

    Urho3D::Vector<TextItem> items_;
    Urho3D::Vector<TextItem>::Iterator insertIt_;
    float timeoutSetting_;
};

}

