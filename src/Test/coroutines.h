#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <map>

#define MINICORO_IMPL
#include "minicoro.h"

class Coroutine {
public:
    Coroutine() = delete;
    Coroutine(mco_coro* co) : m_Co(co) {}
    void Refresh() {
        if (mco_status(m_Co) == MCO_SUSPENDED) mco_resume(m_Co);
    }
    bool IsDead() {
        return mco_status(m_Co) == MCO_DEAD;
    }
    ~Coroutine() {
        mco_destroy(m_Co);
    }
private:
    mco_coro* m_Co = nullptr;
};

class CoroutineFactory {
public:
    static CoroutineFactory& Get() {
        static CoroutineFactory instance;
        return instance;
    }
    Coroutine CreateCoroutine(void(*function)(mco_coro*), uint32_t stack_size = 0) {
        auto it = m_Descs.find({function, stack_size});
        if (m_Descs.find({function, stack_size}) == m_Descs.cend()) {
            auto& desc = m_Descs[{function, stack_size}];
            desc = mco_desc_init(function, stack_size);
            mco_coro* co = nullptr;
            mco_create(&co, &desc);
            return co;
        }

        mco_coro* co = nullptr;
        mco_create(&co, &it->second);
        return co;
    }
private:
    CoroutineFactory() = default;
    std::map<std::pair<void(*)(mco_coro*), int>, mco_desc> m_Descs;
};


//      EXAMPLE
//
//
//constexpr uint32_t FPS = 120;
//
//void delay(mco_coro* coroutine, int k) {
//    for (int i = 0; i < k; i++) {
//        mco_yield(coroutine);
//    }
//}
//
//void coro_entry(mco_coro* coroutine) {
//    printf("coroutine 1\n");
//    delay(coroutine, FPS);
//    printf("coroutine 2\n");
//    delay(coroutine, FPS);
//    printf("coroutine 3\n");
//    delay(coroutine, FPS);
//    printf("coroutine 4\n");
//    delay(coroutine, FPS);
//    printf("coroutine 5\n");
//    delay(coroutine, FPS);
//}
//
//int main() {
//    Coroutine coro = CoroutineFactory::Get().CreateCoroutine(&coro_entry);
//
//    float SPF = 1.f / FPS;
//
//    auto start = std::chrono::high_resolution_clock::now();
//    while (1) {
//        auto end = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<float> dur = end - start;
//
//        if (dur.count() < SPF)
//            continue;
//        else
//            start = std::chrono::high_resolution_clock::now();
//
//        if (coro.IsDead())
//            break;
//        coro.Refresh();
//    }
//
//    return 0;
//}