// 华勤科技版权所有 2010-2011
// 
// 文件名：uiassigner.h
// 功  能：定义核心库的事件分发接口。
// 
// 作  者：汪荣
// 时  间：2010-07-02
// 
// ============================================================================

# ifndef _UIASSIGNER_H_
# define _UIASSIGNER_H_

#include <suicore/uiobject.h>
#include <suicore/uievent.h>
#include <suicore/uiinvokerpriorityqueue.h>
#include <suicore/uistructure.h>
#include <suicore/hwndadapter.h>

#include <util/locker.h>

namespace suic
{

class AssignerFrame;
typedef shared<AssignerFrame> AssignerFramePtr;

class SUICORE_API Assigner : public RefObject
{
public:

    // 当前线程分发器锁
    Mutex _instanceLock;
    bool _continue;

public:

    /// <summary>
    ///     取得当前的分发器对象，一个线程仅仅有一个
    /// </summary>
    /// <returns>AssignerPtr：分配器对象</returns> 
    static AssignerPtr GetCurrentAssigner();

    /// <summary>
    ///     在最后插入一个消息循环
    /// </summary>
    /// <remarks>
    ///     此函数将使用一个AssignerFrame对象来阻塞并分发
    ///     当前窗口消息，当前消息退出后，将进入上一个消息
    ///     分发器
    /// </remarks>
    /// <param name="frame">
    ///     循环执行对象
    /// </param>
    /// <returns>无</returns> 
    static void PushMessageLoop(AssignerFramePtr frame);

    /// <summary>
    ///     创建一个隐藏窗口用于内部消息分发
    /// </summary>
    /// <returns>无</returns> 
    void CreateMessageHwnd();

    /// <summary>
    ///     取得当前的消息分发对象
    /// </summary>
    /// <returns>AssignerFrame：消息分发对象</returns> 
    AssignerFrame* GetCurrentAssignerFrame() const;

    /// <summary>
    ///     执行消息循环
    /// </summary>
    /// <remarks>
    ///     一开始新建一个AssignerFrame对象进行消息处理
    /// </remarks>
    /// <returns>无</returns> 
    void Run();

    /// <summary>
    ///     处理真正的消息分发和处理
    /// </summary>
    /// <remarks>
    ///     消息在这里将会分发到指定的窗口
    /// </remarks>
    /// <param name="msg">
    ///     消息对象
    /// </param>
    /// <returns>无</returns> 
    void TranslateAndAssignMessage(MSG * msg);

    /// <summary>
    ///     接收隐藏窗口消息处理队列
    /// </summary>
    /// <param name="hwnd">
    ///     窗口句柄
    /// </param>
    /// <param name="message">
    ///     消息ID
    /// </param>
    /// <returns>执行后返回值</returns> 
    LRESULT WndProcHook(Handle hwnd, Int32 message, Uint32 wParam, Uint32 lParam, bool& handled);

    /// <summary>
    ///     判断是否有输入消息
    /// </summary>
    /// <returns>true: 有输入消息;false: 无输入消息</returns> 
    bool IsInputPending() const;

    /// <summary>
    ///     执行消息的处理
    /// </summary>
    /// <param name="force">
    ///     强制处理消息
    /// </param>
    /// <returns>true: 消息已处理;false: 放入队列</returns> 
    bool RequestProcessing(bool force=false);

    void ProcessMessageQueue();

    /// <summary>
    ///     同步处理消息
    /// </summary>
    /// <param name="data">
    ///     执行对象
    /// </param>
    /// <param name="timeout">
    ///     等待超时时间
    /// </param>
    /// <returns>返回执行的结果数据</returns> 
    ObjectPtr Invoke(AssignerInvokerPtr data, Int32 timeout);

    /// <summary>
    ///     执行异步消息处理
    /// </summary>
    /// <remarks>
    ///     此函数不进行等待，调用后将马上返回，不论消息执行完与否
    /// </remarks>
    // <param name="data">
    ///     执行对象
    /// </param>
    /// <returns>无</returns> 
    void BeginInvoke(AssignerInvokerPtr data);

    Uint32 GetThreadId() const;

private:

    Assigner();

    void DoRealMessageLoop(AssignerFramePtr frame);
    void DoRealShutdown();
    void InternalShutdown();

    bool IsWindowNull();
    bool DoForegroundMessage();
    bool DoBackgroundMessage();

private:

    // 隐藏窗口的句柄
    Handle _hwnd;
    static AssignerPtr _assigner;
    AssignerFrame* _currFrame;

    Int32 _currentDoType;
    Int32 _msgProcessQueue;
    Int32 _frameDepth;
    bool _exitAllFrames;
    bool _hasFinishedClose;
    bool _hasStartedClose;
    bool _startingClose;
    Uint32 _threadId;

    InvokerPriorityQueuePtr _queue;

    friend class AssignerFrame;
};

inline Uint32 Assigner::GetThreadId() const
{
    return _threadId;
}

class SUICORE_API AssignerFrame : public RefObject
{
public:

    bool _continue;
    bool _exitWhenRequested;

    AssignerFrame(bool exitWhenRequested);
    AssignerFrame(HwndAdapterHookPtr hook);
    AssignerFrame();

    AssignerPtr GetAssigner();
    bool IsContinue();
    void SetContinue(bool value);

    bool HandleHook(MSG* msg);

    void Clear();
    void RegisterHook(HwndAdapterHookPtr hook);
    void RemmoveHook(HwndAdapterHookPtr hook);

protected:

    AssignerPtr _assigner;
    EnumeratorPtr _hooks;
};

};

# endif

