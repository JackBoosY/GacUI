﻿/***********************************************************************
THIS FILE IS AUTOMATICALLY GENERATED. DO NOT MODIFY
DEVELOPER: Zihan Chen(vczh)
***********************************************************************/
#include "VlppReflection.h"
#include "VlppOS.h"
#include "Vlpp.h"
#include "VlppRegex.h"

/***********************************************************************
.\WFLIBRARYCPPHELPER.H
***********************************************************************/
/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_WORKFLOW_LIBRARY_CPPLIBRARY
#define VCZH_WORKFLOW_LIBRARY_CPPLIBRARY


namespace vl
{
	namespace __vwsn
	{
		template<typename T>
		struct RunOnExit
		{
			T* function;

			RunOnExit(T* _function)
				:function(_function)
			{
			}

			~RunOnExit()
			{
				function->operator()();
			}
		};

		template<typename T>
		T* This(T* thisValue)
		{
			CHECK_ERROR(thisValue != nullptr, L"The this pointer cannot be null.");
			return thisValue;
		}

		template<typename T>
		T* Ensure(T* pointer)
		{
			CHECK_ERROR(pointer != nullptr, L"The pointer cannot be null.");
			return pointer;
		}

		template<typename T>
		Ptr<T>& Ensure(Ptr<T>& pointer)
		{
			CHECK_ERROR(pointer != nullptr, L"The pointer cannot be null.");
			return pointer;
		}

		template<typename T>
		Ptr<T> Ensure(Ptr<T>&& pointer)
		{
			CHECK_ERROR(pointer != nullptr, L"The pointer cannot be null.");
			return std::move(pointer);
		}

		template<typename T>
		Nullable<T> Ensure(Nullable<T>&& nullable)
		{
			CHECK_ERROR(nullable, L"The pointer cannot be null.");
			return std::move(nullable);
		}

		template<typename T>
		Nullable<T>& Ensure(Nullable<T>& nullable)
		{
			CHECK_ERROR(nullable, L"The pointer cannot be null.");
			return nullable;
		}

		template<typename T>
		WString ToString(const T& value)
		{
			WString str;
			CHECK_ERROR(reflection::description::TypedValueSerializerProvider<std::remove_cvref_t<T>>::Serialize(value, str), L"Failed to serialize.");
			return str;
		}

		template<typename T>
		T Parse(const WString& str)
		{
			T value;
			CHECK_ERROR(reflection::description::TypedValueSerializerProvider<std::remove_cvref_t<T>>::Deserialize(str, value), L"Failed to serialize.");
			return value;
		}

		template<typename TTo, typename TFrom>
		struct NullableCastHelper
		{
			static Nullable<TTo> Cast(Nullable<TFrom> nullable)
			{
				return Nullable<TTo>(static_cast<TTo>(nullable.Value()));
			}
		};

		template<typename TFrom>
		struct NullableCastHelper<WString, TFrom>
		{
			static Nullable<WString> Cast(Nullable<TFrom> nullable)
			{
				return Nullable<WString>(ToString(nullable.Value()));
			}
		};

		template<typename TTo>
		struct NullableCastHelper<TTo, WString>
		{
			static Nullable<TTo> Cast(Nullable<WString> nullable)
			{
				return Nullable<TTo>(Parse<TTo>(nullable.Value()));
			}
		};

		template<typename TTo, typename TFrom>
		Nullable<TTo> NullableCast(Nullable<TFrom> nullable)
		{
			if (!nullable) return Nullable<TTo>();
			return NullableCastHelper<TTo, TFrom>::Cast(nullable);
		}

		template<typename TTo, typename TFrom>
		TTo* RawPtrCast(TFrom* pointer)
		{
			if (!pointer) return nullptr;
			return pointer->template SafeAggregationCast<TTo>();
		}

		template<typename TTo, typename TFrom>
		Ptr<TTo> SharedPtrCast(TFrom* pointer)
		{
			if (!pointer) return nullptr;
			return pointer->template SafeAggregationCast<TTo>();
		}

		template<typename T>
		reflection::description::Value Box(T&& value)
		{
			return reflection::description::BoxParameter(value);
		}

		template<typename T>
		T Unbox(const reflection::description::Value& value)
		{
			auto unboxed = reflection::description::UnboxParameter<std::remove_cvref_t<T>>(value);
			if (std::is_reference_v<T>)
			{
				CHECK_ERROR(!unboxed.IsOwned(), L"It is impossible to return a reference from a unboxed value, when the unboxing has to call new T(...).");
			}
			return unboxed.Ref();
		}

		template<typename T>
		struct UnboxWeakHelper
		{
		};

		template<typename T>
		struct UnboxWeakHelper<T*>
		{
			static T* Unbox(const reflection::description::Value& value)
			{
				if (value.IsNull()) return nullptr;
				return value.GetRawPtr()->SafeAggregationCast<T>();
			}
		};

		template<typename T>
		struct UnboxWeakHelper<Ptr<T>>
		{
			static Ptr<T> Unbox(const reflection::description::Value& value)
			{
				if (value.IsNull()) return nullptr;
				return value.GetRawPtr()->SafeAggregationCast<T>();
			}
		};

		template<typename T>
		struct UnboxWeakHelper<Nullable<T>>
		{
			static Nullable<T> Unbox(const reflection::description::Value& value)
			{
				if (value.IsNull()) return Nullable<T>();
				auto boxed = value.GetBoxedValue().Cast<reflection::description::IValueType::TypedBox<T>>();
				if (!boxed) return Nullable<T>();
				return Nullable<T>(boxed->value);
			}
		};

		template<typename T>
		T UnboxWeak(const reflection::description::Value& value)
		{
			return UnboxWeakHelper<std::remove_cvref_t<T>>::Unbox(value);
		}

		template<typename T>
		collections::LazyList<T> Range(T begin, T end)
		{
			return collections::Range<T>(begin, end - begin);
		}

		template<typename T>
		bool InSet(const T& value, const collections::LazyList<T>& collection)
		{
			return collection.Any([&](const T& element) {return element == value; });
		}

		template<typename T>
		bool InSet(const T& value, Ptr<reflection::description::IValueReadonlyList> collection)
		{
			return InSet<T>(value, reflection::description::GetLazyList<T>(collection));
		}

		template<typename T, typename U>
		Ptr<T> UnboxCollection(U&& value)
		{
			auto boxedValue = reflection::description::BoxParameter(value);
			return reflection::description::UnboxParameter<Ptr<T>>(boxedValue).Ref();
		}

		template<typename T, typename U>
		Ptr<T> UnboxCollection(const collections::LazyList<U>& value)
		{
			auto boxedValue = reflection::description::BoxParameter(const_cast<collections::LazyList<U>&>(value));
			return reflection::description::UnboxParameter<Ptr<T>>(boxedValue).Ref();
		}

		struct CreateArray
		{
			using IValueArray = reflection::description::IValueArray;

			Ptr<IValueArray>		list;

			CreateArray();
			CreateArray(Ptr<IValueArray> _list);

			CreateArray Resize(vint size)
			{
				list->Resize(size);
				return{ list };
			}

			template<typename T>
			CreateArray Set(vint index, const T& value)
			{
				list->Set(index, Box(value));
				return{ list };
			}
		};

		struct CreateList
		{
			using IValueList = reflection::description::IValueList;

			Ptr<IValueList>			list;

			CreateList();
			CreateList(Ptr<IValueList> _list);

			template<typename T>
			CreateList Add(const T& value)
			{
				list->Add(Box(value));
				return{ list };
			}
		};

		struct CreateObservableList
		{
			using IValueObservableList = reflection::description::IValueObservableList;

			Ptr<IValueObservableList>			list;

			CreateObservableList();
			CreateObservableList(Ptr<IValueObservableList> _list);

			template<typename T>
			CreateObservableList Add(const T& value)
			{
				list->Add(Box(value));
				return{ list };
			}
		};

		struct CreateDictionary
		{
			using IValueDictionary = reflection::description::IValueDictionary;

			Ptr<IValueDictionary>	dictionary;

			CreateDictionary();
			CreateDictionary(Ptr<IValueDictionary> _dictionary);

			template<typename K, typename V>
			CreateDictionary Add(const K& key, const V& value)
			{
				dictionary->Set(Box(key), Box(value));
				return{ dictionary };
			}
		};

		template<typename T>
		struct EventHelper
		{
		};

		template<typename T>
		Ptr<reflection::description::IEventHandler> EventAttach(T& e, typename EventHelper<T>::Handler handler)
		{
			return EventHelper<T>::Attach(e, handler);
		}

		template<typename T>
		bool EventDetach(T& e, Ptr<reflection::description::IEventHandler> handler)
		{
			return EventHelper<T>::Detach(e, handler);
		}

		template<typename T>
		decltype(auto) EventInvoke(T& e)
		{
			return EventHelper<T>::Invoke(e);
		}

		template<typename ...TArgs>
		struct EventHelper<Event<void(TArgs...)>>
		{
			using Handler = const Func<void(TArgs...)>&;

			class EventHandlerImpl : public Object, public reflection::description::IEventHandler
			{
			public:
				Ptr<EventHandler> handler;

				EventHandlerImpl(Ptr<EventHandler> _handler)
					:handler(_handler)
				{
				}

				bool IsAttached()override
				{
					return handler->IsAttached();
				}
			};

			static Ptr<reflection::description::IEventHandler> Attach(Event<void(TArgs...)>& e, Handler handler)
			{
				return MakePtr<EventHandlerImpl>(e.Add(handler));
			}

			static bool Detach(Event<void(TArgs...)>& e, Ptr<reflection::description::IEventHandler> handler)
			{
				auto impl = handler.Cast<EventHandlerImpl>();
				if (!impl) return false;
				return e.Remove(impl->handler);
			}

			static Event<void(TArgs...)>& Invoke(Event<void(TArgs...)>& e)
			{
				return e;
			}
		};
	}
}

#endif

/***********************************************************************
.\WFLIBRARYPREDEFINED.H
***********************************************************************/
/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_WORKFLOW_LIBRARY_PREDEFINED
#define VCZH_WORKFLOW_LIBRARY_PREDEFINED

#include <math.h>

namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Coroutine
***********************************************************************/

			/// <summary>Status of a coroutine.</summary>
			enum class CoroutineStatus
			{
				/// <summary>The coroutine is waiting for resuming.</summary>
				Waiting,
				/// <summary>The coroutine is being executed.</summary>
				Executing,
				/// <summary>The coroutine has stopped.</summary>
				Stopped,
			};

			/// <summary>An object providing input information when resuming a coroutine.</summary>
			class CoroutineResult : public virtual IDescriptable, public Description<CoroutineResult>
			{
			protected:
				Value									result;
				Ptr<IValueException>					failure;

			public:
				/// <summary>Get the object provided to the coroutine. This object is the return value for the pending async operation, like $Await.</summary>
				/// <returns>The object provided to the coroutine.</returns>
				Value									GetResult();

				/// <summary>Set the object provided to the coroutine.</summary>
				/// <param name="value">The object provided to the coroutine.</summary>
				void									SetResult(const Value& value);

				/// <summary>Get the error provided to the coroutine. When it is not nullptr, the return value of <see cref="GetResult"/> is ignored.</summary>
				/// <returns>The error provided to the coroutine.</returns>
				Ptr<IValueException>					GetFailure();

				/// <summary>Set the error provided to the coroutine.</summary>
				/// <param name="value">The error provided to the coroutine.</summary>
				void									SetFailure(Ptr<IValueException> value);
			};

			/// <summary>A coroutine. This is typically created by a Workflow script.</summary>
			class ICoroutine : public virtual IDescriptable, public Description<ICoroutine>
			{
			public:
				/// <summary>Resume the coroutine.</summary>
				/// <param name="raiseException">Set to true to raise an exception that the coroutine encountered. The same exception is accessible by <see cref="GetFailure"/>.</param>
				/// <param name="output">Input for the coroutine in this resuming.</param>
				virtual void							Resume(bool raiseException, Ptr<CoroutineResult> output) = 0;

				/// <summary>Returns the current exception.</summary>
				/// <returns>The current exception. It could cause by the Workflow script that creates this coroutine, or by calling <see cref="Resume"/> when this coroutine is in an inappropriate state.</returns>
				virtual Ptr<IValueException>			GetFailure() = 0;

				/// <summary>Returns the status of the coroutine.</summary>
				/// <returns>The status of the coroutine. <see cref="Resume"/> can be called only when this function returns <see cref="CoroutineStatus::Waiting"/>.</returns>
				virtual CoroutineStatus					GetStatus() = 0;
			};

/***********************************************************************
Coroutine (Enumerable)
***********************************************************************/

			class EnumerableCoroutine : public Object, public Description<EnumerableCoroutine>
			{
			public:
				class IImpl : public virtual IValueEnumerator, public Description<IImpl>
				{
				public:
					virtual void						OnYield(const Value& value) = 0;
					virtual void						OnJoin(Ptr<IValueEnumerable> value) = 0;
				};

				typedef Func<Ptr<ICoroutine>(IImpl*)>	Creator;

				static void								YieldAndPause(IImpl* impl, const Value& value);
				static void								JoinAndPause(IImpl* impl, Ptr<IValueEnumerable> value);
				static void								ReturnAndExit(IImpl* impl);
				static Ptr<IValueEnumerable>			Create(const Creator& creator);
			};

/***********************************************************************
Coroutine (Async)
***********************************************************************/

			/// <summary>Status of am async operation.</summary>
			enum class AsyncStatus
			{
				/// <summary>The async operation is ready to execute.</summary>
				Ready,
				/// <summary>The async operation is being executed.</summary>
				Executing,
				/// <summary>The async operation has stopped.</summary>
				Stopped,
			};

			/// <summary>A context providing communication between the caller and the async operation.</summary>
			class AsyncContext : public virtual IDescriptable, public Description<AsyncContext>
			{
			protected:
				SpinLock								lock;
				bool									cancelled = false;
				Value									context;

			public:
				/// <summary>Create a context.</summary>
				/// <param name="_context">Set the initial return value for <see cref="GetContext"/> (optional)..</param>
				AsyncContext(const Value& _context = {});
				~AsyncContext();

				/// <summary>Test if the current async operation is expected to cancel.</summary>
				/// <returns>Returns true if the current async operation is expected to cancel.</returns>
				/// <remarks>
				/// This function is accessible by "$.IsCancelled" in an $Async coroutine.
				/// A cancelable async operation should check this value when it is able to stop properly, and stop when it is true.
				/// </remarks>
				bool									IsCancelled();

				/// <summary>Set <see cref="IsCancelled"/> to true.</summary>
				/// <returns>Returns true when this operation succeeded.</returns>
				bool									Cancel();

				/// <summary>Returns a value that is accessible in Workflow script by "$.Context" in an $Async coroutine.</summary>
				/// <returns>A value that is accessible in Workflow script by "$.Context" in an $Async coroutine.</returns>
				const description::Value&				GetContext();

				/// <summary>Set a value that is accessible F</summary>
				/// <param name="value">A value that is accessible in Workflow script by "$.Context" in an $Async coroutine.</param>
				void									SetContext(const description::Value& value);
			};

			/// <summary>An async operation.</summary>
			class IAsync : public virtual IDescriptable, public Description<IAsync>
			{
			public:
				/// <summary>Get the status of this async operation.</summary>
				/// <returns>The status of this async operation.</returns>
				virtual AsyncStatus						GetStatus() = 0;

				/// <summary>Run this async operation.</summary>
				/// <returns>Returns true when this operation succeeded. This function cannot be called twice on the same object.</returns>
				/// <param name="callback">A callback to execute when the async operation finished.</param>
				/// <param name="context">A context object that is accessible in Workflow script by "$" in an $Async coroutine (optional).</param>
				virtual bool							Execute(const Func<void(Ptr<CoroutineResult>)>& callback, Ptr<AsyncContext> context = nullptr) = 0;

				/// <summary>Create an async operation that finished after a specified moment of time.</summary>
				/// <returns>Returns the created async operation.</returns>
				/// <param name="milliseconds">The time in milliseconds to wait. It counts from when this function is called, not from when this async operation is executed.</param>
				static Ptr<IAsync>						Delay(vint milliseconds);
			};

			/// <summary>A promise object that controls a <see cref="IFuture"/> object.</summary>
			class IPromise : public virtual IDescriptable, public Description<IPromise>
			{
			public:
				/// <summary>Mark the <see cref="IFuture"/> object as finished by providing a value.</summary>
				/// <returns>Returns true when this operation succeeded. Multiple calls to <see cref="SendResult"/> and <see cref="SendFailure"/> cause a failure.</returns>
				/// <param name="result">The result of the <see cref="IFuture"/> object.</param>
				virtual bool							SendResult(const Value& result) = 0;

				/// <summary>Mark the <see cref="IFuture"/> object as finished by providing an exception.</summary>
				/// <returns>Returns true when this operation succeeded. Multiple calls to <see cref="SendResult"/> and <see cref="SendFailure"/> cause a failure.</returns>
				/// <param name="failure">The exception of the <see cref="IFuture"/> object.</param>
				virtual bool							SendFailure(Ptr<IValueException> failure) = 0;
			};

			/// <summary>An async operation in the future-promise pattern.</summary>
			class IFuture : public virtual IAsync, public Description<IFuture>
			{
			public:
				/// <summary>Get the <see cref="IPromise"/> that controls this future object.</summary>
				/// <returns>The <see cref="IPromise"/> that controls this future object.</returns>
				virtual Ptr<IPromise>					GetPromise() = 0;

				/// <summary>Create a future object.</summary>
				/// <returns>The created future object.</returns>
				static Ptr<IFuture>						Create();
			};

			/// <summary>A scheduler that controls how async operations are executed. It needs to be implemented and attached to threads that run async operations.</summary>
			/// <remarks>See <a href="/workflow/lang/coroutine_async.html">Async Coroutine</a> for more information.</remarks>
			class IAsyncScheduler : public virtual IDescriptable, public Description<IAsyncScheduler>
			{
			public:
				/// <summary>Called when a callback needs to be executed in any thread.</summary>
				/// <param name="callback">The callback to execute.</param>
				/// <remarks>
				/// You can decide which thread to execute.
				/// For GacUI, the scheduler that attached to the UI thread will execute this callback in the UI thread.
				/// </remarks>
				virtual void							Execute(const Func<void()>& callback) = 0;

				/// <summary>Called when a callback needs to be executed in another thread.</summary>
				/// <param name="callback">The callback to execute.</param>
				/// <remarks>
				/// You can decide which thread to execute except the current one.
				/// For GacUI, the scheduler that attached to any thread will execute this callback in a random background thread.
				/// </remarks>
				virtual void							ExecuteInBackground(const Func<void()>& callback) = 0;

				/// <summary>Called when a callback needs to be executed in any thread after a specified moment of time.</summary>
				/// <param name="callback">The callback to execute.</param>
				/// <param name="milliseconds">The time in milliseconds to wait.</param>
				/// <remarks>
				/// You can decide which thread to execute.
				/// For GacUI, the scheduler that attached to the UI thread will execute this callback in the UI thread.
				/// </remarks>
				virtual void							DelayExecute(const Func<void()>& callback, vint milliseconds) = 0;

				/// <summary>Attach a scheduler for all threads.</summary>
				/// <param name="scheduler">The scheduler to attach.</param>
				static void								RegisterDefaultScheduler(Ptr<IAsyncScheduler> scheduler);

				/// <summary>Attach a scheduler for the current thread.</summary>
				/// <param name="scheduler">The scheduler to attach.</param>
				static void								RegisterSchedulerForCurrentThread(Ptr<IAsyncScheduler> scheduler);

				/// <summary>Detach the scheduler for all threads.</summary>
				/// <returns>The previously attached scheduler.</returns>
				static Ptr<IAsyncScheduler>				UnregisterDefaultScheduler();

				/// <summary>Detach the scheduler for the current thread.</summary>
				/// <returns>The previously attached scheduler.</returns>
				static Ptr<IAsyncScheduler>				UnregisterSchedulerForCurrentThread();

				/// <summary>Get the attached scheduler for the current thread.</summary>
				/// <returns>The attached scheduler. If there is no scheduler that is attached to this particular thread, the default scheduler kicks in.</returns>
				static Ptr<IAsyncScheduler>				GetSchedulerForCurrentThread();
			};

			class AsyncCoroutine : public Object, public Description<AsyncCoroutine>
			{
			public:
				class IImpl : public virtual IAsync, public Description<IImpl>
				{
				public:
					virtual Ptr<IAsyncScheduler>		GetScheduler() = 0;
					virtual Ptr<AsyncContext>			GetContext() = 0;
					virtual void						OnContinue(Ptr<CoroutineResult> output) = 0;
					virtual void						OnReturn(const Value& value) = 0;
				};

				typedef Func<Ptr<ICoroutine>(IImpl*)>	Creator;

				static void								AwaitAndRead(IImpl* impl, Ptr<IAsync> value);
				static void								ReturnAndExit(IImpl* impl, const Value& value);
				static Ptr<AsyncContext>				QueryContext(IImpl* impl);
				static Ptr<IAsync>						Create(const Creator& creator);
				static void								CreateAndRun(const Creator& creator);
			};

/***********************************************************************
Coroutine (State Machine)
***********************************************************************/

			class StateMachine : public Object, public AggregatableDescription<StateMachine>
			{
#ifdef VCZH_DESCRIPTABLEOBJECT_WITH_METADATA
				friend struct CustomTypeDescriptorSelector<StateMachine>;
#endif
			protected:
				bool									stateMachineInitialized = false;
				bool									stateMachineStopped = false;
				vint									stateMachineInput = -1;
				Ptr<ICoroutine>							stateMachineCoroutine;

				void									ResumeStateMachine();
			public:
				StateMachine();
				~StateMachine();

				CoroutineStatus							GetStateMachineStatus();
			};

/***********************************************************************
Libraries
***********************************************************************/

			/// <summary>system::Sys includes a lot of utility functions for type conversion, string operations and date time operations for a Workflow script.</summary>
			class Sys : public Description<Sys>
			{
			public:
				static vint			Int32ToInt(vint32_t value)							{ return (vint)value; }
				static vint			Int64ToInt(vint64_t value)							{ return (vint)value; }
				static vint32_t		IntToInt32(vint value)								{ return (vint32_t)value; }
				static vint64_t		IntToInt64(vint value)								{ return (vint64_t)value; }
				
				static vuint		UInt32ToUInt(vuint32_t value)						{ return (vuint)value; }
				static vuint		UInt64ToUInt(vuint64_t value)						{ return (vuint)value; }
				static vuint32_t	UIntToUInt32(vuint value)							{ return (vuint32_t)value; }
				static vuint64_t	UIntToUInt64(vuint value)							{ return (vuint64_t)value; }

				static vint			Len(const WString& value)							{ return value.Length(); }
				static WString		Left(const WString& value, vint length)				{ return value.Left(length); }
				static WString		Right(const WString& value, vint length)			{ return value.Right(length); }
				static WString		Mid(const WString& value, vint start, vint length)	{ return value.Sub(start, length); }
				static vint			Find(const WString& value, const WString& substr)	{ return INVLOC.FindFirst(value, substr, Locale::Normalization::None).key; }
				static WString		UCase(const WString& value)							{ return wupper(value); }
				static WString		LCase(const WString& value)							{ return wlower(value); }

				static WString		LoremIpsumTitle(vint bestLength)					{ return vl::LoremIpsumTitle(bestLength); }
				static WString		LoremIpsumSentence(vint bestLength)					{ return vl::LoremIpsumSentence(bestLength); }
				static WString		LoremIpsumParagraph(vint bestLength)				{ return vl::LoremIpsumParagraph(bestLength); }

#define DEFINE_COMPARE(TYPE) static vint Compare(TYPE a, TYPE b);
				REFLECTION_PREDEFINED_PRIMITIVE_TYPES(DEFINE_COMPARE)
				DEFINE_COMPARE(DateTime)
#undef DEFINE_COMPARE

				static DateTime		GetLocalTime();
				static DateTime		GetUtcTime();
				static DateTime		ToLocalTime(DateTime dt);
				static DateTime		ToUtcTime(DateTime dt);
				static DateTime		Forward(DateTime dt, vuint64_t milliseconds);
				static DateTime		Backward(DateTime dt, vuint64_t milliseconds);
				static DateTime		CreateDateTime(vint year, vint month, vint day);
				static DateTime		CreateDateTime(vint year, vint month, vint day, vint hour, vint minute, vint second, vint milliseconds);
				static DateTime		CreateDateTime(vuint64_t filetime);

				static Ptr<IValueEnumerable>		ReverseEnumerable(Ptr<IValueEnumerable> value);
			};

			/// <summary>system::Math includes math functions for a Workflow script.</summary>
			class Math : public Description<Math>
			{
			public:
				static double		Pi()							{ return ASin(1) * 2; }

				static vint8_t		Abs(vint8_t value)				{ return value > 0 ? value : -value; }
				static vint16_t		Abs(vint16_t value)				{ return value > 0 ? value : -value; }
				static vint32_t		Abs(vint32_t value)				{ return value > 0 ? value : -value; }
				static vint64_t		Abs(vint64_t value)				{ return value > 0 ? value : -value; }
				static float		Abs(float value)				{ return value > 0 ? value : -value; }
				static double		Abs(double value)				{ return value > 0 ? value : -value; }
				
#define DEFINE_MINMAX(TYPE)\
				static TYPE Min(TYPE a, TYPE b);\
				static TYPE Max(TYPE a, TYPE b);\

				REFLECTION_PREDEFINED_PRIMITIVE_TYPES(DEFINE_MINMAX)
				DEFINE_MINMAX(DateTime)
#undef DEFINE_MINMAX

				static double		Sin(double value)				{ return sin(value); }
				static double		Cos(double value)				{ return cos(value); }
				static double		Tan(double value)				{ return tan(value); }
				static double		ASin(double value)				{ return asin(value); }
				static double		ACos(double value)				{ return acos(value); }
				static double		ATan(double value)				{ return atan(value); }
				static double		ATan2(double x, double y)		{ return atan2(y, x); }

				static double		Exp(double value)				{ return exp(value);  }
				static double		LogN(double value)				{ return log(value); }
				static double		Log10(double value)				{ return log10(value); }
				static double		Log(double value, double base)	{ return log(value) / log(base); }
				static double		Pow(double value, double power)	{ return pow(value, power); }
				static double		Ceil(double value)				{ return ceil(value); }
				static double		Floor(double value)				{ return floor(value); }
				static double		Round(double value)				{ return round(value); }
				static double		Trunc(double value)				{ return trunc(value); }
			};

			/// <summary>system::Math includes localization awared formatting operations for a Workflow script.</summary>
			/// <remarks>
			/// <p>
			/// There are three locales that reflect the configuration of the operating system:
			/// <ul>
			///     <li><b>Invariant</b>: An invariant locale for general languages.</li>
			///     <li><b>System</b>: Locale for the operating system, including the file system.</li>
			///     <li><b>User</b>: Locale for UI of the operating system.</li>
			/// </ul>
			/// </p>
			/// </remarks>
			class Localization : public Description<Localization>
			{
			public:
				static Locale							Invariant();
				static Locale							System();
				static Locale							User();
				static collections::LazyList<Locale>	Locales();

				static collections::LazyList<WString>	GetShortDateFormats(Locale locale);
				static collections::LazyList<WString>	GetLongDateFormats(Locale locale);
				static collections::LazyList<WString>	GetYearMonthDateFormats(Locale locale);
				static collections::LazyList<WString>	GetLongTimeFormats(Locale locale);
				static collections::LazyList<WString>	GetShortTimeFormats(Locale locale);

				static WString							GetShortDayOfWeekName(Locale locale, vint dayOfWeek);
				static WString							GetLongDayOfWeekName(Locale locale, vint dayOfWeek);
				static WString							GetShortMonthName(Locale locale, vint month);
				static WString							GetLongMonthName(Locale locale, vint month);

				static WString							FormatDate(Locale locale, const WString& format, DateTime date);
				static WString							FormatTime(Locale locale, const WString& format, DateTime date);
				static WString							FormatNumber(Locale locale, const WString& number);
				static WString							FormatCurrency(Locale locale, const WString& number);
			};
		}
	}
}

#endif


/***********************************************************************
.\WFLIBRARYREFLECTION.H
***********************************************************************/
/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_WORKFLOW_LIBRARY_REFLECTION
#define VCZH_WORKFLOW_LIBRARY_REFLECTION


namespace vl
{
	namespace reflection
	{
		namespace description
		{

/***********************************************************************
Predefined Types
***********************************************************************/

#ifndef VCZH_DEBUG_NO_REFLECTION

#define WORKFLOW_LIBRARY_TYPES(F)\
			F(Sys)							\
			F(Math)							\
			F(Localization)					\
			F(CoroutineStatus)				\
			F(CoroutineResult)				\
			F(ICoroutine)					\
			F(EnumerableCoroutine::IImpl)	\
			F(EnumerableCoroutine)			\
			F(AsyncStatus)					\
			F(AsyncContext)					\
			F(IAsync)						\
			F(IPromise)						\
			F(IFuture)						\
			F(IAsyncScheduler)				\
			F(AsyncCoroutine::IImpl)		\
			F(AsyncCoroutine)				\
			F(StateMachine)					\

			WORKFLOW_LIBRARY_TYPES(DECL_TYPE_INFO)

#endif

/***********************************************************************
Interface Implementation Proxy (Implement)
***********************************************************************/

#ifdef VCZH_DESCRIPTABLEOBJECT_WITH_METADATA

#pragma warning(push)
#pragma warning(disable:4250)

			BEGIN_INTERFACE_PROXY_NOPARENT_SHAREDPTR(ICoroutine)

				void Resume(bool raiseException, Ptr<CoroutineResult> output)override
				{
					INVOKE_INTERFACE_PROXY(Resume, raiseException, output);
				}

				Ptr<IValueException> GetFailure()override
				{
					INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetFailure);
				}

				CoroutineStatus GetStatus()override
				{
					INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetStatus);
				}
			END_INTERFACE_PROXY(ICoroutine)

			BEGIN_INTERFACE_PROXY_NOPARENT_SHAREDPTR(IAsync)

				AsyncStatus GetStatus()override
				{
					INVOKEGET_INTERFACE_PROXY_NOPARAMS(GetStatus);
				}

				bool Execute(const Func<void(Ptr<CoroutineResult>)>& callback, Ptr<AsyncContext> context)override
				{
					INVOKEGET_INTERFACE_PROXY(Execute, callback, context);
				}
			END_INTERFACE_PROXY(IAsync)

#pragma warning(pop)

#endif

/***********************************************************************
LoadPredefinedTypes
***********************************************************************/

			extern bool										WfLoadLibraryTypes();
		}
	}
}

#endif

