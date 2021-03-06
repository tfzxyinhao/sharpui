// 华勤科技版权所有 2010-2011
// 
// 文件名：CaptionBar.h
// 功  能：实现Windows标准的标题。
// 
// 作  者：汪荣
// 时  间：2010-07-02
// 
// ============================================================================

# ifndef _UICAPTIONBAR_H_
# define _UICAPTIONBAR_H_

#include <sui/StackLayout.h>
#include <sui/Button.h>
#include <sui/TextBlock.h>

namespace ui
{

class SHARPUI_API CaptionBar : public suic::Panel
{
public:

    CaptionBar();
    virtual ~CaptionBar();

    DECLAREBUILD(CaptionBar)

    /// <summary>
    ///     隐藏系统最小化按钮
    /// </summary>
    /// <param name="value">true:隐藏；false；显示</param>
    /// <returns>无</returns>
    void HideMinBox(bool value);

    /// <summary>
    ///     隐藏系统最大化按钮
    /// </summary>
    /// <param name="value">true:隐藏；false；显示</param>
    /// <returns>无</returns>
    void HideMaxBox(bool value);

    /// <summary>
    ///     隐藏系统关闭按钮
    /// </summary>
    /// <param name="value">true:隐藏；false；显示</param>
    /// <returns>无</returns>
    void HideCloseBox(bool value);

    /// <summary>
    ///     取得标题的布局界面元素
    /// </summary>
    /// <remarks>
    ///     标题采用栈式布局，默认标题文本填满剩余空间
    /// </remarks>
    /// <returns>布局元素对象</returns>
    suic::PanelPtr GetLayout();

protected:

    virtual void AddLogicalChild(suic::Element* child);

    virtual suic::Size MeasureOverride(const suic::Size& size);
    virtual suic::Size ArrangeOverride(const suic::Size& size);

    virtual void OnInitialized();
    virtual void OnStyleUpdated();

    virtual void OnRender(suic::DrawingContext * drawing);

    virtual void OnMouseLeftButtonDown(suic::MouseEventArg& e);
    virtual void OnMouseLeftButtonDbclk(suic::MouseEventArg& e);
    virtual void OnMouseLeftButtonUp(suic::MouseEventArg& e);
    virtual void OnMouseEnter(suic::MouseEventArg& e);
    virtual void OnMouseMove(suic::MouseEventArg& e);
    virtual void OnMouseLeave(suic::MouseEventArg& e);

protected:

    // 布局对象
    ui::StackLayout _layout;
    // 标题文本
    ui::TextBlockPtr _title;
    // 最大化按钮
    suic::ElementPtr _maxBtn;

};

}

# endif
