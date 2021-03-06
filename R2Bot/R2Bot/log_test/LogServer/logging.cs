// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.1
//
// <auto-generated>
//
// Generated from file `logging.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//


using _System = global::System;
using _Microsoft = global::Microsoft;

#pragma warning disable 1591

namespace IceCompactId
{
}

namespace Logging
{
    [_System.Runtime.InteropServices.ComVisible(false)]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1704")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1707")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1709")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1710")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1711")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1715")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1716")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1720")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1722")]
    [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Naming", "CA1724")]
    public partial interface RemoteLog : Ice.Object, RemoteLogOperations_, RemoteLogOperationsNC_
    {
    }
}

namespace Logging
{
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public delegate void Callback_RemoteLog_log();
}

namespace Logging
{
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public interface RemoteLogPrx : Ice.ObjectPrx
    {
        void log(string name, string time, string msg);
        void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__);

        Ice.AsyncResult<Logging.Callback_RemoteLog_log> begin_log(string name, string time, string msg);
        Ice.AsyncResult<Logging.Callback_RemoteLog_log> begin_log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> ctx__);

        Ice.AsyncResult begin_log(string name, string time, string msg, Ice.AsyncCallback cb__, object cookie__);
        Ice.AsyncResult begin_log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> ctx__, Ice.AsyncCallback cb__, object cookie__);

        void end_log(Ice.AsyncResult r__);
    }
}

namespace Logging
{
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public interface RemoteLogOperations_
    {
        void log(string name, string time, string msg, Ice.Current current__);
    }

    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public interface RemoteLogOperationsNC_
    {
        void log(string name, string time, string msg);
    }
}

namespace Logging
{
    [_System.Runtime.InteropServices.ComVisible(false)]
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public sealed class RemoteLogPrxHelper : Ice.ObjectPrxHelperBase, RemoteLogPrx
    {
        #region Synchronous operations

        public void log(string name, string time, string msg)
        {
            log(name, time, msg, null, false);
        }

        public void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            log(name, time, msg, context__, true);
        }

        private void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            Ice.Instrumentation.InvocationObserver observer__ = IceInternal.ObserverHelper.get(this, __log_name, context__);
            int cnt__ = 0;
            try
            {
                while(true)
                {
                    Ice.ObjectDel_ delBase__ = null;
                    try
                    {
                        delBase__ = getDelegate__(false);
                        RemoteLogDel_ del__ = (RemoteLogDel_)delBase__;
                        del__.log(name, time, msg, context__, observer__);
                        return;
                    }
                    catch(IceInternal.LocalExceptionWrapper ex__)
                    {
                        handleExceptionWrapper__(delBase__, ex__, observer__);
                    }
                    catch(Ice.LocalException ex__)
                    {
                        handleException__(delBase__, ex__, true, ref cnt__, observer__);
                    }
                }
            }
            finally
            {
                if(observer__ != null)
                {
                    observer__.detach();
                }
            }
        }

        #endregion

        #region Asynchronous operations

        public Ice.AsyncResult<Logging.Callback_RemoteLog_log> begin_log(string name, string time, string msg)
        {
            return begin_log(name, time, msg, null, false, null, null);
        }

        public Ice.AsyncResult<Logging.Callback_RemoteLog_log> begin_log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> ctx__)
        {
            return begin_log(name, time, msg, ctx__, true, null, null);
        }

        public Ice.AsyncResult begin_log(string name, string time, string msg, Ice.AsyncCallback cb__, object cookie__)
        {
            return begin_log(name, time, msg, null, false, cb__, cookie__);
        }

        public Ice.AsyncResult begin_log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> ctx__, Ice.AsyncCallback cb__, object cookie__)
        {
            return begin_log(name, time, msg, ctx__, true, cb__, cookie__);
        }

        private const string __log_name = "log";

        public void end_log(Ice.AsyncResult r__)
        {
            end__(r__, __log_name);
        }

        private Ice.AsyncResult<Logging.Callback_RemoteLog_log> begin_log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> ctx__, bool explicitContext__, Ice.AsyncCallback cb__, object cookie__)
        {
            IceInternal.OnewayOutgoingAsync<Logging.Callback_RemoteLog_log> result__ = new IceInternal.OnewayOutgoingAsync<Logging.Callback_RemoteLog_log>(this, __log_name, log_completed__, cookie__);
            if(cb__ != null)
            {
                result__.whenCompletedWithAsyncCallback(cb__);
            }
            try
            {
                result__.prepare__(__log_name, Ice.OperationMode.Normal, ctx__, explicitContext__);
                IceInternal.BasicStream os__ = result__.startWriteParams__(Ice.FormatType.DefaultFormat);
                os__.writeString(name);
                os__.writeString(time);
                os__.writeString(msg);
                result__.endWriteParams__();
                result__.send__(true);
            }
            catch(Ice.LocalException ex__)
            {
                result__.exceptionAsync__(ex__);
            }
            return result__;
        }

        private void log_completed__(Logging.Callback_RemoteLog_log cb__)
        {
            if(cb__ != null)
            {
                cb__();
            }
        }

        #endregion

        #region Checked and unchecked cast operations

        public static RemoteLogPrx checkedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            RemoteLogPrx r = b as RemoteLogPrx;
            if((r == null) && b.ice_isA(ice_staticId()))
            {
                RemoteLogPrxHelper h = new RemoteLogPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RemoteLogPrx checkedCast(Ice.ObjectPrx b, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            RemoteLogPrx r = b as RemoteLogPrx;
            if((r == null) && b.ice_isA(ice_staticId(), ctx))
            {
                RemoteLogPrxHelper h = new RemoteLogPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RemoteLogPrx checkedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA(ice_staticId()))
                {
                    RemoteLogPrxHelper h = new RemoteLogPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static RemoteLogPrx checkedCast(Ice.ObjectPrx b, string f, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA(ice_staticId(), ctx))
                {
                    RemoteLogPrxHelper h = new RemoteLogPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static RemoteLogPrx uncheckedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            RemoteLogPrx r = b as RemoteLogPrx;
            if(r == null)
            {
                RemoteLogPrxHelper h = new RemoteLogPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static RemoteLogPrx uncheckedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            RemoteLogPrxHelper h = new RemoteLogPrxHelper();
            h.copyFrom__(bb);
            return h;
        }

        public static readonly string[] ids__ =
        {
            "::Ice::Object",
            "::Logging::RemoteLog"
        };

        public static string ice_staticId()
        {
            return ids__[1];
        }

        #endregion

        #region Marshaling support

        protected override Ice.ObjectDelM_ createDelegateM__()
        {
            return new RemoteLogDelM_();
        }

        protected override Ice.ObjectDelD_ createDelegateD__()
        {
            return new RemoteLogDelD_();
        }

        public static void write__(IceInternal.BasicStream os__, RemoteLogPrx v__)
        {
            os__.writeProxy(v__);
        }

        public static RemoteLogPrx read__(IceInternal.BasicStream is__)
        {
            Ice.ObjectPrx proxy = is__.readProxy();
            if(proxy != null)
            {
                RemoteLogPrxHelper result = new RemoteLogPrxHelper();
                result.copyFrom__(proxy);
                return result;
            }
            return null;
        }

        #endregion
    }
}

namespace Logging
{
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public interface RemoteLogDel_ : Ice.ObjectDel_
    {
        void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__, Ice.Instrumentation.InvocationObserver observer__);
    }
}

namespace Logging
{
    [_System.Runtime.InteropServices.ComVisible(false)]
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public sealed class RemoteLogDelM_ : Ice.ObjectDelM_, RemoteLogDel_
    {
        public void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__, Ice.Instrumentation.InvocationObserver observer__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("log", Ice.OperationMode.Normal, context__, observer__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.startWriteParams(Ice.FormatType.DefaultFormat);
                    os__.writeString(name);
                    os__.writeString(time);
                    os__.writeString(msg);
                    og__.endWriteParams();
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                if(og__.hasResponse())
                {
                    try
                    {
                        if(!ok__)
                        {
                            try
                            {
                                og__.throwUserException();
                            }
                            catch(Ice.UserException ex__)
                            {
                                throw new Ice.UnknownUserException(ex__.ice_name(), ex__);
                            }
                        }
                        og__.readEmptyParams();
                    }
                    catch(Ice.LocalException ex__)
                    {
                        throw new IceInternal.LocalExceptionWrapper(ex__, false);
                    }
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }
    }
}

namespace Logging
{
    [_System.Runtime.InteropServices.ComVisible(false)]
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public sealed class RemoteLogDelD_ : Ice.ObjectDelD_, RemoteLogDel_
    {
        [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1031")]
        public void log(string name, string time, string msg, _System.Collections.Generic.Dictionary<string, string> context__, Ice.Instrumentation.InvocationObserver observer__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "log", Ice.OperationMode.Normal, context__);
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                RemoteLog servant__ = null;
                try
                {
                    servant__ = (RemoteLog)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                servant__.log(name, time, msg, current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.getServant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(_System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }
    }
}

namespace Logging
{
    [_System.Runtime.InteropServices.ComVisible(false)]
    [_System.CodeDom.Compiler.GeneratedCodeAttribute("slice2cs", "3.5.1")]
    public abstract class RemoteLogDisp_ : Ice.ObjectImpl, RemoteLog
    {
        #region Slice operations

        public void log(string name, string time, string msg)
        {
            log(name, time, msg, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void log(string name, string time, string msg, Ice.Current current__);

        #endregion

        #region Slice type-related members

        public static new readonly string[] ids__ = 
        {
            "::Ice::Object",
            "::Logging::RemoteLog"
        };

        public override bool ice_isA(string s)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override bool ice_isA(string s, Ice.Current current__)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override string[] ice_ids()
        {
            return ids__;
        }

        public override string[] ice_ids(Ice.Current current__)
        {
            return ids__;
        }

        public override string ice_id()
        {
            return ids__[1];
        }

        public override string ice_id(Ice.Current current__)
        {
            return ids__[1];
        }

        public static new string ice_staticId()
        {
            return ids__[1];
        }

        #endregion

        #region Operation dispatch

        [_System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1011")]
        public static Ice.DispatchStatus log___(RemoteLog obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Normal, current__.mode);
            IceInternal.BasicStream is__ = inS__.startReadParams();
            string name;
            string time;
            string msg;
            name = is__.readString();
            time = is__.readString();
            msg = is__.readString();
            inS__.endReadParams();
            obj__.log(name, time, msg, current__);
            inS__.writeEmptyParams__();
            return Ice.DispatchStatus.DispatchOK;
        }

        private static string[] all__ =
        {
            "ice_id",
            "ice_ids",
            "ice_isA",
            "ice_ping",
            "log"
        };

        public override Ice.DispatchStatus dispatch__(IceInternal.Incoming inS__, Ice.Current current__)
        {
            int pos = _System.Array.BinarySearch(all__, current__.operation, IceUtilInternal.StringUtil.OrdinalStringComparer);
            if(pos < 0)
            {
                throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
            }

            switch(pos)
            {
                case 0:
                {
                    return ice_id___(this, inS__, current__);
                }
                case 1:
                {
                    return ice_ids___(this, inS__, current__);
                }
                case 2:
                {
                    return ice_isA___(this, inS__, current__);
                }
                case 3:
                {
                    return ice_ping___(this, inS__, current__);
                }
                case 4:
                {
                    return log___(this, inS__, current__);
                }
            }

            _System.Diagnostics.Debug.Assert(false);
            throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
        }

        #endregion

        #region Marshaling support

        protected override void writeImpl__(IceInternal.BasicStream os__)
        {
            os__.startWriteSlice(ice_staticId(), -1, true);
            os__.endWriteSlice();
        }

        protected override void readImpl__(IceInternal.BasicStream is__)
        {
            is__.startReadSlice();
            is__.endReadSlice();
        }

        #endregion
    }
}
