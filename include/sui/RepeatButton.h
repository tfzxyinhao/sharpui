// 华勤科技版权所有 2010-2011
// 
// 文件名：RepeatButton.h
// 功  能：实现频率按钮，支持三态切换。
// 
// 作  者：汪荣
// 时  间：2010-07-02
// 
// ============================================================================

# ifndef _UIREPEATBUTTON_H_
# define _UIREPEATBUTTON_H_

#include <sui/sharpuiconfig.h>
#include <sui/Button.h>

namespace ui
{

class SHARPUI_API RepeatButton : public Button
{
public:

    RepeatButton();
    virtual ~RepeatButton();

    DECLAREBUILD(RepeatButton)

    // 鼠标左键压下时的延迟时间
    void SetDelay(int iDelay);

    // 发送click事件的间隔事件
    void SetInterval(int iInterval);

public:

    virtual void OnInitialized();
    virtual void OnUnloaded(suic::LoadedEventArg& e);

    virtual void OnRender(suic::DrawingContext * drawing);

    virtual void OnMouseLeftButtonDown(suic::MouseEventArg& e);
    virtual void OnMouseLeftButtonUp(suic::MouseEventArg& e);
    virtual void OnMouseEnter(suic::MouseEventArg& e);
    virtual void OnMouseLeave(suic::MouseEventArg& e);
    virtual void OnClick(suic::RoutedEventArg& e);

    virtual void OnTimer(int id);

    virtual void OnRepeated(bool bMouseIn);

protected:

    // 重复频率，毫秒
    int m_iDelay;
    int m_iInterval;
    suic::Uint32 m_iTimer300;
    suic::Uint32 m_iTimer301;

};

}

# endif
