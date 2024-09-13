#pragma once

template <typename T>
class Singleton {
public:
    static T *getInstance();
};

template <typename T>
T *Singleton<T>::getInstance() {
    static T *instance = new T();
    return instance;
}

#define SINGLETON(Class)                                                                           \
private:                                                                                           \
    friend class Singleton<Class>;                                                                 \
                                                                                                   \
public:                                                                                            \
    static Class *getInstance() {                                                                  \
        return Singleton<Class>::getInstance();                                                    \
    }

#define Q_PROPERTY_AUTO_P(TYPE, M)                                                                 \
    Q_PROPERTY(TYPE M MEMBER m_##M NOTIFY M##Changed)                                              \
public:                                                                                            \
    Q_SIGNAL void M##Changed();                                                                    \
    void M(TYPE in_##M) {                                                                          \
        if (in_##M == m_##M)                                                                       \
            return;                                                                                \
        m_##M = in_##M;                                                                            \
        Q_EMIT M##Changed();                                                                       \
    }                                                                                              \
    TYPE M() const {                                                                               \
        return m_##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE m_##M = nullptr;

#define Q_PROPERTY_READONLY_AUTO_P(TYPE, M)                                                        \
    Q_PROPERTY(TYPE M READ M NOTIFY M##Changed FINAL)                                              \
public:                                                                                            \
    Q_SIGNAL void M##Changed();                                                                    \
    void M(TYPE in_##M) {                                                                          \
        if (in_##M == m_##M)                                                                       \
            return;                                                                                \
        m_##M = in_##M;                                                                            \
        Q_EMIT M##Changed();                                                                       \
    }                                                                                              \
    TYPE M() const {                                                                               \
        return m_##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE m_##M = nullptr;

#define Q_PROPERTY_AUTO(TYPE, M)                                                                   \
    Q_PROPERTY(TYPE M MEMBER m_##M NOTIFY M##Changed)                                              \
public:                                                                                            \
    Q_SIGNAL void M##Changed();                                                                    \
    void M(const TYPE &in_##M) {                                                                   \
        if (in_##M == m_##M)                                                                       \
            return;                                                                                \
        m_##M = in_##M;                                                                            \
        Q_EMIT M##Changed();                                                                       \
    }                                                                                              \
    TYPE M() const {                                                                               \
        return m_##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE m_##M;

#define Q_PROPERTY_READONLY_AUTO(TYPE, M)                                                          \
    Q_PROPERTY(TYPE M READ M NOTIFY M##Changed FINAL)                                              \
public:                                                                                            \
    Q_SIGNAL void M##Changed();                                                                    \
    void M(const TYPE &in_##M) {                                                                   \
        if (in_##M == m_##M)                                                                       \
            return;                                                                                \
        m_##M = in_##M;                                                                            \
        Q_EMIT M##Changed();                                                                       \
    }                                                                                              \
    TYPE M() const {                                                                               \
        return m_##M;                                                                              \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    TYPE m_##M;
