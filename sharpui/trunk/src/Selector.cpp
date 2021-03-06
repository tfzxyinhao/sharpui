// ======================================================================
//
// Copyright (c) 2008-2012 汪荣, Inc. All rights reserved.
//
// Sharpui库源码遵循CPL协议进行开源，任何个人或团体在此协议上可以自由使用。
//
// ======================================================================

//////////////////////////////////////////////////////////////////////////////
// Selector.cpp

#include <sui/Selector.h>

namespace ui
{

SelectionChangedEventArg::SelectionChangedEventArg()
: suic::RoutedEventArg(suic::ObjectPtr())
{
}

void SelectionChangedEventArg::AddItem(suic::ObjectPtr item, bool bAdd)
{
    if (bAdd)
    {
        _addItems.Add(item);
    }
    else
    {
        _removeItems.Add(item);
    }
}

ItemContentList* SelectionChangedEventArg::AddedItems()
{
    return &_addItems;
}

ItemContentList* SelectionChangedEventArg::RemovedItems()
{
    return &_removeItems;
}
// ============================================================================
// Selector，Item的基类。
// ============================================================================

Selector::Selector()
    : _selectMode(SelectionMode::Single)
    , _focusItem(NULL)
{
}

Selector::~Selector()
{
}

Selector* Selector::SelectorFromItem(suic::Element* pElem)
{
    suic::Element* pParent = pElem->LogicalParent();

    while (pParent)
    {
        Selector* pRet = dynamic_cast<Selector*>(pParent);

        if (pRet)
        {
            return pRet;
        }

        pParent = pParent->LogicalParent();
    }

    return NULL;
}

void Selector::SelectItem(suic::ObjectPtr item, bool selected)
{
    suic::ElementPtr pElem(item);
    Selector* pSelector = Selector::SelectorFromItem(pElem.get());

    if (!pElem || !pSelector)
    {
        ;
    }
    else
    {
        ItemSelectionEventArg es(selected, 0);

        pElem->WriteFlag(CoreFlags::IsSelected, selected);
        pSelector->OnItemSelected(item, es);
    }
}

int Selector::SelectedIndex()
{
    return -1;
}

void Selector::SelectedIndex(int iIndex)
{
    suic::ElementPtr viewPtr(GetItems()->GetItem(iIndex));

    SelectItem(viewPtr.get(), true);
}

suic::ObjectPtr Selector::SelectedItem()
{
    suic::ObjectPtr ret;

    if (_selectedItems.GetCount() > 0)
    {
        ret = _selectedItems.GetAt(0);
    }
    
    return ret;
}

void Selector::SelectedItem(suic::ObjectPtr obj)
{
    int index = GetItems()->IndexOf(obj);

    if (index >= 0)
    {
        SelectedIndex(index);
    }
}

void Selector::UnselectAllItems()
{
    for (int i = 0; i < _selectedItems.GetCount(); ++i)
    {
        suic::ElementPtr item(_selectedItems.GetAt(i));

        item->WriteFlag(CoreFlags::IsSelected, false);
    }

    _selectedItems.Reset();

    if (_focusItem)
    {
        _focusItem->WriteFlag(CoreFlags::IsSelected, false);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//
void Selector::OnItemSelected(suic::ObjectPtr item, ItemSelectionEventArg& e)
{
    suic::Element* oldFocus = _focusItem;
    SelectionChangedEventArg ec;
    suic::Element* pItem = dynamic_cast<suic::Element*>(item.get());

    if (e.IsSelected())
    {
        if (_focusItem != pItem)
        {
            _focusItem = pItem;
        }

        if (ItemSelected)
        {
            ItemSelected(pItem);
        }
    }
    else
    {
        if (_focusItem == pItem)
        {
            _focusItem = NULL;
        }

        if (ItemUnselected)
        {
            ItemUnselected(pItem);
        }
    }

    ec.AddItem(item, e.IsSelected());
    OnSelectionChanged(ec);

    if (_focusItem && _focusItem != oldFocus)
    {
        _focusItem->Focus();
    }
}

void Selector::OnSelectionChanged(SelectionChangedEventArg& e)
{
    if (e.RemovedItems()->GetCount() > 0)
    {
        for (int i = 0; i < e.RemovedItems()->GetCount(); ++i)
        {
            _selectedItems.Remove(e.RemovedItems()->GetAt(i));
        }
    }

    if (e.AddedItems()->GetCount() > 0)
    {
        if (_selectMode == SelectionMode::Single)
        {
            if (_selectedItems.GetCount() > 0
                && _selectedItems.GetAt(0) != e.AddedItems()->GetAt(0))
            {
                suic::ElementPtr pElem(_selectedItems.GetAt(0));

                Selector::SelectItem(_selectedItems.GetAt(0), false);

                pElem->InvalidateVisual();
            }

            _selectedItems.Reset();
        }

        for (int i = 0; i < e.AddedItems()->GetCount(); ++i)
        {
            suic::ObjectPtr itemPtr = e.AddedItems()->GetAt(i);

            _selectedItems.Add(itemPtr);
        }
    }

    if (SelectionChanged)
    {
        SelectionChanged(this, e);
    }
}

void Selector::ScrollByUpDown(suic::Element* pElem, bool bUp)
{
    double val = _scrollView->VerticalScrollBar()->GetScrollPos();
    double step = _scrollView->VerticalScrollBar()->GetScrollStep();
    double pos = (double)pElem->GetDesiredSize().cy / step + 0.5;

    if (bUp)
    {
        pos = -pos;
    }

    _scrollView->ScrollToVerticalPos(val + pos);
    _scrollView->InvalidateArrange();
    _scrollView->InvalidateVisual();    
}

void Selector::OnInitialized()
{
    __super::OnInitialized();

    for (int i = 0; i < GetItems()->GetCount(); ++i)
    {
        suic::FrameworkElementPtr itemPtr(GetItems()->GetItem(i));
        suic::ObjectPtr obj = itemPtr->GetStyle()->GetValue(_T("IsSelected"));

        if (obj && obj->ToBool())
        {
            SelectedIndex(i);
        }
    }
}

void Selector::OnTextInput(suic::KeyEventArg& e)
{
    __super::OnTextInput(e);

    double val = _scrollView->HorizontalScrollBar()->GetScrollSize();
    double step = _scrollView->HorizontalScrollBar()->GetScrollStep();

    if (e.IsLeftArrow())
    {
        _scrollView->ScrollToHorizontalPos(val - 1);
    }
    else if (e.IsRightArrow())
    {
        _scrollView->ScrollToHorizontalPos(val + 1);
    }

    e.Handled(true);
}

void Selector::OnKeyDown(suic::KeyEventArg& e)
{
    if (e.IsLeftArrow() || e.IsRightArrow())
    {
        OnTextInput(e);
        _scrollView->LineLeft();
    }
    else if (e.IsUpArrow())
    {
        if (!_focusItem)
        {
            if (GetMaxVisualIndex() > 0)
            {
                SelectItem(_panel->GetVisualChild(GetMaxVisualIndex() - 1), true);
            }
        }
        else
        {
            int index = _panel->GetVisualChildIndex(_focusItem);

            if (index > GetMinVisualIndex())
            {
                SelectItem(_panel->GetVisualChild(index - 1), true);

                if (index == GetMinVisualIndex() + 1)
                {
                    ScrollByUpDown(GetLogicalChild(index - 1), true);
                }
            }
            else
            {
                index = _panel->GetStartLogicalIndex();

                if (index > 0)
                {
                    ScrollByUpDown(GetLogicalChild(index - 1), true);
                }
                else
                {
                    ScrollByUpDown(GetLogicalChild(0), true);
                }
            }
        }
    }
    else if (e.IsDownArrow())
    {
        if (!_focusItem)
        {
            if (_panel->GetVisualChildrenCount() > 0)
            {
                SelectItem(GetVisualChild(0), true);
            }
        }
        else
        {
            int index = _panel->GetVisualChildIndex(_focusItem);

            if (index < GetMaxVisualIndex() - 1)
            {
                SelectItem(_panel->GetVisualChild(index + 1), true);

                if (index == GetMaxVisualIndex() - 2)
                {
                    ScrollByUpDown(GetLogicalChild(index + 1), false);
                }
            }
            else
            {
                index = _panel->GetEndLogicalIndex();

                if (index < GetItemsCount() - 1)
                {
                    ScrollByUpDown(GetLogicalChild(index + 1), false);
                }
                else
                {
                    ScrollByUpDown(GetLogicalChild(GetItemsCount() - 1), false);
                }
            }
        }
    }
    else if (e.IsPageup())
    {
        ;
    }
    else if (e.IsPagedown())
    {
        ;
    }
    else if (e.IsHomeKey())
    {
        ;
    }
    else if (e.IsEndKey())
    {
        ;
    }
    else
    {
        __super::OnKeyDown(e);
        return;
    }

    e.Handled(true);
}

};
