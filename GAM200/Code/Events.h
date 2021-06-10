///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Events.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Templates for function pointers for event handling
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <list>
#include <utility>

#ifndef _EVENTS_H_
#define _EVENTS_H_


///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
namespace EventZone
{
  template<typename T>
  class EventHandlerTempBase
  {
  public:
    EventHandlerTempBase() {}
    virtual ~EventHandlerTempBase() {}
    virtual bool IsBoundToSameFunctionAs(EventHandlerTempBase<T>*) = 0;

    bool IsSameType(EventHandlerTempBase<T>* otherHandler)
    {
      if (otherHandler == nullptr || typeid(*this) != typeid(*otherHandler))
        return false;

      return true;
    }
  };

  ///one arguement event handlers
  template<typename T>
  class EventHandlerTemp : public EventHandlerTempBase<T>
  {
  public:
    virtual void OnEvent(T&) = 0;
  };

  template<typename T>
  class EventHandlerTempNonMember : public EventHandlerTemp<T>
  {
  public:
    EventHandlerTempNonMember(void(*functionP)(T&)) : mpfunctionP(functionP)
    {
    }

    virtual void OnEvent(T& event)
    {
      mpfunctionP(event);
    }

    virtual bool IsBoundToSameFunctionAs(EventHandlerTempBase<T>* otherHandler)
    {
      EventHandlerTempBase<T> *temp = dynamic_cast<EventHandlerTempBase<T>*>(this);
      if (!(temp->IsSameType(otherHandler)))
        return false;

      EventHandlerTempNonMember<T>* handlerCast = dynamic_cast<EventHandlerTempNonMember<T>*>(otherHandler);
      if (!handlerCast)
        return false;

      return this->mpfunctionP == handlerCast->mpfunctionP;
    }

  private:
    void(*mpfunctionP)(T&);
  };

  ///to handle member function calls
  template<typename T, typename U>
  class EventHandlerTempMember : public EventHandlerTemp<T>
  {
  public:
    EventHandlerTempMember(void(U::*memberFunctionP)(T&), U* thisPtr) : mpCallerInstance(thisPtr), mpMemberFunctionP(memberFunctionP)
    {
    }

    virtual void OnEvent(T& event)
    {
      if (mpCallerInstance)
        (mpCallerInstance->*mpMemberFunctionP)(event);
    }

    virtual bool IsBoundToSameFunctionAs(EventHandlerTempBase<T> *otherHandler)
    {
      if (!(this->IsSameType(otherHandler)))
        return false;

      EventHandlerTempMember<T, U> *handlerCast = dynamic_cast<EventHandlerTempMember<T, U>*>(otherHandler);
      if (handlerCast == nullptr)
        return false;

      return this->mpCallerInstance == handlerCast->mpCallerInstance && this->mpMemberFunctionP == handlerCast->mpMemberFunctionP;
    }

  private:
    U *mpCallerInstance;
    void(U::*mpMemberFunctionP)(T&);
  };

  ///no arguments event handlers for special void events
  template<>
  class EventHandlerTemp<void> : public EventHandlerTempBase<void>
  {
  public:
    virtual void OnEvent() = 0;
  };

  template<>
  class EventHandlerTempNonMember<void> : public EventHandlerTemp<void>
  {
  public:
    EventHandlerTempNonMember(void(*functionP)()) : mpFunctionP(functionP)
    {
    }

    virtual void OnEvent()
    {
      mpFunctionP();
    }

    virtual bool IsBoundToSameFunctionAs(EventHandlerTempBase<void>* otherHandler)
    {
      if (!IsSameType(otherHandler))
        return false;

      EventHandlerTempNonMember<void>* handlerCast = dynamic_cast<EventHandlerTempNonMember<void>*>(otherHandler);
      if (handlerCast == nullptr)
        return false;

      return this->mpFunctionP == handlerCast->mpFunctionP;
    }

  private:
    void(*mpFunctionP)();
  };

  template<typename U>
  class EventHandlerTempMember<void, U> : public EventHandlerTemp<void>
  {
  public:
    EventHandlerTempMember(void(U::*memberFunctionP)(), U* thisPtr) : mpCallerInstance(thisPtr), mpMemberFunctionP(memberFunctionP)
    {
    }

    virtual void OnEvent()
    {
      if (mpCallerInstance)
        (mpCallerInstance->*mpMemberFunctionP)();
    }

    virtual bool IsBoundToSameFunctionAs(EventHandlerTempBase<void>* otherHandler)
    {
      if (!IsSameType(otherHandler))
        return false;

      EventHandlerTempMember<void, U> *handlerCast = dynamic_cast<EventHandlerTempMember<void, U>*>(otherHandler);
      if (handlerCast == nullptr)
        return false;

      return this->mpCallerInstance == handlerCast->mpCallerInstance && this->mpMemberFunctionP == handlerCast->mpMemberFunctionP;
    }

  private:
    U *mpCallerInstance;
    void(U::*mpMemberFunctionP)();
  };


  ///-------------------
  // EventHandler class
  ///-------------------

  class EventHandler
  {
  public:
    ///one argument event handlers
    template<typename T> //1
    static EventHandlerTemp<T> * Bind(void(*nonMemberFunction)(T&))
    {
      return new EventHandlerTempNonMember<T>(nonMemberFunction);
    }

    template<typename T, typename U> //2
    static EventHandlerTemp<T> * Bind(void(U::*memberFunction)(T&), U* thisPtr)
    {
      return new EventHandlerTempMember<T, U>(memberFunction, thisPtr);
    }

    ///no argument event handlers
    static EventHandlerTemp<void> * Bind(void(*nonMemberFunction)())
    {
      return new EventHandlerTempNonMember<void>(nonMemberFunction);
    }

    template<typename U> //4
    static EventHandlerTemp<void> * Bind(void(U::*memberFunction)(), U *thisPtr)
    {
      return new EventHandlerTempMember<void, U>(memberFunction, thisPtr);
    }

  private:
    EventHandler();
  };

  ///-------------------
  // Event class
  ///-------------------
  template<typename T>
  class EventBase
  {
  public:
    EventBase() {}

    virtual ~EventBase()
    {
      ///USE EVENT BOOST FOR THREAD SAFETY HERE???
      for (auto it = this->eventHandlers_.begin(); it != this->eventHandlers_.end(); ++it)
      {
        EventHandlerTemp<T> *handlerP = *it;
        if (handlerP)
        {
          delete handlerP;
          handlerP = 0;
        }
      }
      this->eventHandlers_.clear();
    }

    EventBase<T>& operator+=(EventHandlerTemp<T> *handler)
    {
      if (handler)
        this->eventHandlers_.push_back(handler); ///this one 

      return *this;
    }

    EventBase<T>& operator-=(EventHandlerTemp<T> *handler)
    {
      if (handler == nullptr)
        return *this;

      for (auto it = this->eventHandlers_.begin(); it != this->eventHandlers_.end(); ++it)
      {
        EventHandlerTemp<T> *handlerP = *it;
        if (handler->IsBoundToSameFunctionAs(handlerP))
        {
          EventHandlerTemp<T> *foundHandler = *it;
          if (foundHandler != nullptr)
          {
            delete foundHandler;
            foundHandler = 0;
          }

          this->eventHandlers_.erase(it);
          break;
        }
      }

      if (handler != nullptr)
      {
        delete handler;
        handler = 0;
      }

      return *this;
    }

  private:
    EventBase(const EventBase&);
    EventBase& operator=(const EventBase&);

  protected:
    std::list<EventHandlerTemp<T>*> eventHandlers_;
  };

  template<typename T>
  class Event : public EventBase<T>
  {
  public:
    void operator()(T& eventData)
    {
      for (auto it = this->eventHandlers_.begin(); it != this->eventHandlers_.end(); ++it)
      {
        EventHandlerTemp<T> *handlerP = *it;
        if (handlerP != nullptr)
          handlerP->OnEvent(eventData);
      }
    }

  };

  template<>
  class Event<void> : public EventBase<void>
  {
  public:
    void operator()()
    {
      for (std::list<EventHandlerTemp<void>*>::iterator it = eventHandlers_.begin(); it != eventHandlers_.end(); ++it)
      {
        EventHandlerTemp<void> *handlerP = *it;
        if (handlerP != nullptr)
          handlerP->OnEvent();
      }
    }
  };
}

//#include "Events.cpp"

#endif // !_EVENTS_H_
///---------------------------------FUNCTIONS-----------------------------------

