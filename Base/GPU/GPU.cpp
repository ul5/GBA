#include "GPU.h"

Base::GPU::GPU(Base::MMU *m) : mmu(m) {
    gpu_thread = std::thread(&GPU::start_gpu, std::ref(*this));
    printf("Continuing execution\n");
}

void Base::GPU::start_gpu() {
    int dec_by = 0;
    
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(cycles > 1232) {
            dec_by += 1232;
            mmu->w8(0x04000006, mmu->r8(0x04000006) + 1);
        }

        cycle_lock.lock();
        cycles -= dec_by;
        cycle_lock.unlock();
        dec_by = 0;
    }
}