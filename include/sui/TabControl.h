// 华勤科技版权所有 2010-2011
// 
// 文件名：TabControl.h
// 功  能：实现标准b的tab控件。
// 
// 作  者：汪荣
// 时  间：2010-08-31
// 
// ============================================================================

# ifndef _UITABCONTROL_H_
# define _UITABCONTROL_H_

#include <sui/sharpuiconfig.h>
#include <sui/TabItem.h>
#include <sui/Selector.h>
#include <sui/StackPanel.h>

namespace ui
{

///==================================================
/// Tab控件，表头可以任意定制元素进行组合.
///
class SHARPUI_API TabControl : public Selector
{
public:

    TabControl();
    virtual ~TabControl();

    DECLAREBUILD(TabControl);

    suic::ObjectPtr SelectedContent() const;

protected:
    
    virtual suic::Size MeasureOverride(const suic::Size& size);
    virtual suic::Size ArrangeOverride(const suic::Size& size);

    virtual void OnInitialized();
    virtual void OnLoaded(suic::LoadedEventArg& e);

    virtual void OnRender(suic::DrawingContext * drawing);

    virtual void OnItemsChanged(NotifyContainerChangedArg& e);    
    virtual void OnItemSelected(suic::ObjectPtr item, ItemSelectionEventArg& e);

    virtual void OnPreviewMouseLeftButtonDown(suic::MouseEventArg& e);
    virtual void OnPreviewMouseLeftButtonUp(suic::MouseEventArg& e);
    virtual void OnMouseEnter(suic::MouseEventArg& e);
    virtual void OnMouseLeave(suic::MouseEventArg& e);

protected:

    // 列表头是否采用平均值计算
    bool _isAverage;
    // Tab项的最大高度
    int _headerHeight;
    // Tab项布局器
    StackPanel _headerPanel;
};

typedef suic::shared<TabControl> TabControlPtr;

};

# endif
