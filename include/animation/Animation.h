// 华勤科技版权所有 2010-2012
// 
// 文件名：animation.h
// 功  能：定义动画基类，任何动画将从这个类继承。
// 
// 作  者：汪荣
// 时  间：2012-02-01
// 
// ============================================================================

# ifndef _UIANIMATION_H_
# define _UIANIMATION_H_

#include <animation/timeline.h>
#include <suicore/uiframeworkelement.h>
#include <suicore/drawing/UIDrawing.h>

namespace suic
{

class SUICORE_API Animation : public Timeline
{
public:

    Animation();
    virtual ~Animation();

    bool IsRunning() const;
    bool IsPause() const;

    bool IsEndOf() const;

    /// <summary>
    ///     获取下一帧动画的间隔时间
    /// </summary>
    /// <remarks>
    ///     每种动画类型都应该根据相应的算法计算动画帧间隔
    /// </remarks>
    /// <returns>下一帧间隔时间</returns>
    virtual double GetInterval();

    /// <summary>
    ///     播放一帧动画
    /// </summary>
    /// <remarks>
    ///     调用此方法计算当前时间点的动画帧或计算动画元素的状态
    /// </remarks>
    /// <param name="pElem">目标元素</param>
    /// <returns>无</returns>
    virtual void OnMeasure(FrameworkElement* pElem);

    /// <summary>
    ///     计算当前动画帧
    /// </summary>
    /// <remarks>
    ///     此方法作为属性动画不会被调用
    /// </remarks>
    /// <param name="pImage">关键帧对象</param>
    /// <returns>无</returns>
    virtual void OnFrame(ImagePtr& pImage);

    /// <summary>
    ///     动画开始时回调
    /// </summary>
    /// <param name="pElem">目标元素</param>
    /// <returns>无</returns>
    virtual void OnStart(FrameworkElement* pElem);

    /// <summary>
    ///     动画结束时回调
    /// </summary>
    /// <param name="pElem">目标元素</param>
    /// <returns>无</returns>
    virtual void OnEnd(suic::FrameworkElement* pElem);

    /// <summary>
    ///     动画暂停时回调
    /// </summary>
    /// <returns>无</returns>
    virtual void OnPause();

    /// <summary>
    ///     动画重新播放时回调
    /// </summary>
    /// <returns>无</returns>
    virtual void OnResume();

protected:

    // 动画状态
    // 0：停止；1：运行中；2：暂停
    int _state;

    // 动画是否结束，由具体动画计算
    bool _isEndOf;
};

typedef shared<Animation> AnimationPtr;

};

# endif
