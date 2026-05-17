#include <cstdio>
#include <chrono>
#include <map>

#define MINICORO_IMPL
#include "minicoro.h"

using coroutine_pointer = mco_coro*;

class Coroutine {
public:
    Coroutine() = default;

    Coroutine(void(*f)(coroutine_pointer), uint32_t stack_size = 0) {
        mco_desc desc = mco_desc_init(f, stack_size);
        mco_create(&m_Co, &desc);
    }

    Coroutine(const Coroutine& other) = delete;
    Coroutine& operator=(const Coroutine& other) = delete;

    Coroutine(Coroutine&& other) noexcept {
        if (m_Co) mco_destroy(m_Co);
        m_Co = other.m_Co;
        other.m_Co = nullptr;
    }

    Coroutine& operator=(Coroutine&& other) noexcept {
        if (m_Co) mco_destroy(m_Co);
        m_Co = other.m_Co;
        other.m_Co = nullptr;

        return *this;
    }

    bool IsDead() const {
        return mco_status(m_Co) == MCO_DEAD;
    }

    void Resume(void* args) const {
        m_Co->user_data = args;
        if (mco_status(m_Co) == MCO_SUSPENDED)
            mco_resume(m_Co);
    }

    static void Yield(coroutine_pointer coroutine) {
        mco_yield(coroutine);
    }

    ~Coroutine() {
        if (m_Co) mco_destroy(m_Co);
    }
private:
    mco_coro* m_Co = nullptr;
};


/*
void delay(coroutine_pointer coroutine) {
    for (int i = 0; i < FPS; i++) {
        Coroutine::Yield(coroutine);
    }
}

void coro_entry(coroutine_pointer coroutine) {
    printf("coroutine 1\n");
    delay(coroutine);
    printf("coroutine 2\n");
    delay(coroutine);
    printf("coroutine 3\n");
    delay(coroutine);
    printf("coroutine 4\n");
    delay(coroutine);
    printf("coroutine 5\n");
    delay(coroutine);
}

int main() {
    float SPF = 1.f / FPS;

    Coroutine coroutine = &coro_entry;

    auto start = std::chrono::high_resolution_clock::now();
    while (1) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> dur = end - start;

        if (dur.count() < SPF) continue;
        start = std::chrono::high_resolution_clock::now();

        if (coroutine.IsDead()) {
            coroutine = &coro_entry;
        }
        coroutine.Resume();
    }

    return 0;
}
*/