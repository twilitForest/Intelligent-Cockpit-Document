import re
import os
import matplotlib.pyplot as plt



def get_app_cpu(path_ap):
    CPU_adas = []
    PID0 = 'com.ford.sync.controllerlauncher'
    RE0 = re.compile(r'M.*?(\d+(\.\d+)).*')
    with open(path_ap) as f:
        for line in f:
            line = line.strip()
            if line.endswith(PID0):
                match0 = RE0.search(line)
                if match0:
                    cpu_adas = line.split()[8]
                    CPU_adas.append(float(cpu_adas))
        # print(CPU_adas)
        print('max_cpu=', max(CPU_adas))
        average_cpu = sum(CPU_adas) / len(CPU_adas)
        print('average=',average_cpu)



def get_ap_mem(path_mem):
    MEM_adas = []
    PID0 = 'com.ford.sync.controllerlauncher'
    with open(path_mem) as f:
        for line in f:
            line = line.strip()
            if line.endswith(PID0):
                aiservice_mem1 = line.split()[3]
                if 'K' in aiservice_mem1:
                    aiservice_mem2 = int(aiservice_mem1.strip('K'))

                MEM_adas.append(aiservice_mem2)

        # print(MEM_adas)
        print('max_mem', max(MEM_adas))
        average_mem = sum(MEM_adas) / len(MEM_adas)
        print('avg_mem',average_mem)


def get_app_Gpu(path_gp):
    GPU = []
    with open(path_gp) as f:
        for line in f:
            if "percentage" in line:
                line = line.strip()
                gpu = line.split()[-1]
                if '%' in gpu:
                    gpu1 = float(gpu.strip('%'))
                GPU.append(gpu1)
            # print(GPU)
        print('max_gpu=', max(GPU))
        average_gpu = sum(GPU) / len(GPU)
        print('average_gpu=', average_gpu)


if __name__ == '__main__':
    f_path = r"D:\Script\StressTestScript\0103\android_cpu.log"
    m_path = r"D:\Script\StressTestScript\0103\android_mem.log"
    g_path = r"D:\Script\StressTestScript\0103\GPU.txt"
    get_app_cpu(f_path)
    get_ap_mem(m_path)
    get_app_Gpu(g_path)



