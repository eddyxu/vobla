/*
 * Copyright 2014 (c) Lei Xu <eddyxu@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <glog/logging.h>
#if defined(linux) || defined(__linux__)
#include <linux/fs.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/disk.h>
#include <libproc.h>
#endif /* __APPLE__ */
#include <string>
#include "vobla/gutil/stringprintf.h"
#include "vobla/sysinfo.h"

namespace vobla {

using std::string;
const int BUFSIZE = 1024;

static double cpufreq_ = 0;

double SysInfo::GetCpuFrequency() {
  if (cpufreq_)
    return cpufreq_;

  FILE* fp;
  char buffer[BUFSIZE];
#if defined(linux) || defined(__linux__)
  if ((fp = fopen("/proc/cpuinfo", "r")) == nullptr) {
    perror("get_cpu_freq: Can not get cpu information");
    return 0;
  }

  for (int i = 6; i > 0; i--) {
    if (fgets(buffer, BUFSIZE-1, fp) == nullptr) {
      perror("get_cpu_freq: can not get cpu information");
      fclose(fp);
      return 0;
    }
  }

  if (fscanf(fp, "%*s%*s%*s%lf", &cpufreq_) != 1) {
    perror("get_cpu_freq: can not get cpu information");
    fclose(fp);
    return 0;
  }
  cpufreq_ *= 1000000;
#elif defined(__APPLE__) || defined(__NetBSD__) || defined(__FreeBSD__)

  if ((fp = popen("sysctl -a", "r")) == nullptr) {
    perror("cpu_freq(): get cpu frequency");
    return 0;
  }

  while (fgets(buffer, BUFSIZE-1, fp) != nullptr) {
#if defined(__APPLE__)
    if (strnstr(buffer, "hw.cpufrequency:", BUFSIZE) == nullptr)
#else
    if (strnstr(buffer, "dev.cpu.0.freq:", BUFSIZE) == nullptr)
#endif /* FreeBSD & NetBSD */
      continue;

    sscanf(buffer, "%*s%lf", &cpufreq_);  // NOLINT
#if defined(__FreeBSD__) || defined(__NetBSD__)
    cpufreq_ *= 1000000;
#endif /* FreeBSD / NetBSD */
    break;
  }
#endif /* __APPLE__ || __FreeBSD__ || __NetBSD__ */
  fclose(fp);
  return cpufreq_;
}


/**
 * Get available number of CPUs
 *
 * @see http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
 */
int SysInfo::GetNumCpus() {
  int num_cpus = 0;
#if defined(linux) || defined(__linux__) || \
  defined(__APPLE__) || defined(__FreeBSD__)
  num_cpus= sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(WIN32)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  num_cpus = sysinfo.dwNumberOfProcessors;
#endif
  return num_cpus;
}

/*
inline uint64_t rdtsc() {
  uint32_t lo, hi;
  __asm__ __volatile__
    ("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t) hi << 32 | lo;
}

double get_cur_time(void) {
  return rdtsc() / cpufreq_;
}
*/

pid_t SysInfo::GetParentPid(pid_t pid) {
  if (pid == 0) {
    return 0;
  }
#if defined(__APPLE__)
  struct kinfo_proc info;
  size_t length = sizeof(struct kinfo_proc);
  int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, pid };
  if (sysctl(mib, 4, &info, &length, nullptr, 0) < 0)
    return -1;
  if (length == 0)
    return -1;
  return info.kp_eproc.e_ppid;
#elif defined(__linux__)
  char buffer[BUFSIZE];
  string proc_filepath = StringPrintf("/proc/%d/status", pid);
  pid_t parent = -1;
  FILE* fp = fopen(proc_filepath.c_str(), "r");
  if (!fp) {
    VLOG(1) << "Failed to open " << proc_filepath << strerror(errno);
    return parent;
  }
  while (fgets(buffer, BUFSIZE, fp) != nullptr) {
    if (strncmp(buffer, "PPid:", 5) == 0) {
      sscanf(buffer, "PPid: %d", &parent);  // NOLINT
      break;
    }
  }
  fclose(fp);
  return parent;
#endif /* __linux__ */
}

int SysInfo::GetProcessName(pid_t pid, string* name) {
#if defined(__APPLE__)
  char buffer[BUFSIZE];
  int ret = 0;
  ret = proc_name(pid, buffer, BUFSIZE);
  *name = buffer;
  if (ret) {
    return ret;
  }
#elif defined(__linux__)
  char proc_filepath[BUFSIZE];
  char line[BUFSIZE];
  memset(proc_filepath, 0, BUFSIZE);
  memset(line, 0, BUFSIZE);
  snprintf(proc_filepath, BUFSIZE, "/proc/%d/cmdline", pid);
  FILE* fp = fopen(proc_filepath, "r");
  if (!fp) {
    perror("get_process_name: failed to open /proc/<pid>/cmdline");
    return -1;
  }
  if (fgets(line, BUFSIZE, fp) == nullptr) {
    perror("get_process_name: failed to extract cmdline from "
           "/proc/<pid>/cmdline");
    fclose(fp);
    return -1;
  }
  char* pos = strrchr(line, '/');
  if (pos) {
    *name = pos+1;
  } else {
    *name = line;
  }
  fclose(fp);
#else  /* __linux__ */
#error "Unsupported platform"
#endif
  return 0;
}

}  // namespace vobla
