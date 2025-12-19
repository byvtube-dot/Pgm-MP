#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <type_traits>

template <class T>
class Property
{
private:
    LPVOID  m_lpParam = nullptr;
    T       m_Value = { 0 };
    std::function<void(LPVOID)> ConditionEvent = nullptr;
    std::function<void(const T&, const T&, LPVOID)> BeforeEvent = nullptr;
    std::function<void(const T&, const T&, LPVOID)> AfterEvent = nullptr;

public:
    Property() = default;

    Property(std::function<void(LPVOID)> conditionEvent,
        std::function<void(const T&, const T&, LPVOID)> beforeEvent,
        std::function<void(const T&, const T&, LPVOID)> afterEvent)
        : BeforeEvent(beforeEvent), AfterEvent(afterEvent), ConditionEvent(conditionEvent) {}

    operator T() const
    {
        if (nullptr != ConditionEvent)
            ConditionEvent(m_lpParam);

        return m_Value;
    }

    Property& operator=(const T& newValue)
    {
        const T oldValue = m_Value;

        if (nullptr != BeforeEvent)
            BeforeEvent(oldValue, newValue, m_lpParam);

        m_Value = newValue;

        if (nullptr != AfterEvent)
            AfterEvent(oldValue, newValue, m_lpParam);

        return *this;
    }

    bool operator==(const T& other) const 
    {
        if (nullptr != ConditionEvent)
            ConditionEvent(m_lpParam);

        return m_Value == other;
    }

    bool operator!=(const T& other) const 
    {
        if (nullptr != ConditionEvent)
            ConditionEvent(m_lpParam);

        return m_Value != other;
    }

    bool operator==(const Property& other) const 
    {
        if (nullptr != ConditionEvent)
            ConditionEvent(m_lpParam);

        return m_Value == other.m_Value;
    }

    bool operator!=(const Property& other) const 
    {
        if (nullptr != ConditionEvent)
            ConditionEvent(m_lpParam);

        return m_Value != other.m_Value;
    }

    friend bool operator==(const T& lhs, const Property& rhs)
    {
        if (nullptr != rhs.ConditionEvent)
            rhs.ConditionEvent(rhs.m_lpParam);

        return lhs == rhs.m_Value;
    }

    friend bool operator!=(const T& lhs, const Property& rhs)
    {
        if (nullptr != rhs.ConditionEvent)
            rhs.ConditionEvent(rhs.m_lpParam);

        return lhs != rhs.m_Value;
    }

    void Init(std::function<void(LPVOID)> conditionEvent,
        std::function<void(const T&, const T&, LPVOID)> beforeEvent,
        std::function<void(const T&, const T&, LPVOID)> afterEvent, PVOID lpParam = nullptr)
    {
        ConditionEvent = conditionEvent;
        BeforeEvent = beforeEvent;
        AfterEvent = afterEvent;
        m_lpParam = lpParam;
    }
};