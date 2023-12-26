//
// Created by bkg2k on 26/12/23.
//
#pragma once

#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <cstdio>

#ifdef __x86_64
  #include <ucontext.h>
  #include <sys/ucontext.h>
#elif defined(__aarch64)
  #include <asm/sigcontext.h>
	#include <ucontext.h>
	#include <sys/ucontext.h>
#endif

static void OnSIGSEGV(int signum,siginfo_t* info,void* ptr);

void InitSegvCatcher()
{
  struct sigaction act { };
  sigemptyset(&act.sa_mask);
  act.sa_sigaction = OnSIGSEGV;
  act.sa_flags = SA_SIGINFO;
  if(sigaction(SIGSEGV, &act, nullptr)<0 || sigaction(SIGFPE, &act, nullptr)<0)
  {
    printf("Error assigning signal handlers!\n");
    abort();
  }
}

static void OnSIGSEGV(int signum, siginfo_t *info, void *ptr)
{
  static int iTime;
  if (iTime++ >= 1)
  {
    printf("ReEnter %s is not allowed!\n", __FUNCTION__);
    abort();
  }

  printf("SEGFAULT reading/writing at %llX\n", (long long int)(info->si_addr));

  void* addresses[64];
  void* functions[64];
  memset(&functions, 0, sizeof(functions));
  int nSize = backtrace(addresses, sizeof(addresses)/sizeof(addresses[0]));
  backtrace_symbols(functions, sizeof(functions)/sizeof(functions[0]));
  for (int i = nSize; --i >= 3; )
    printf("Catched when running code at %llX (%s)\n", (long long int)addresses[i],
           (const char*)functions[i] != nullptr ? (const char*)functions[i] : "unknown function");

  if (ptr != nullptr)
  {
    #ifdef __x86_64
    ucontext_t* ptrUC = (ucontext_t*)ptr;
    long long int* pgregs = (long long int*)(&(ptrUC->uc_mcontext.gregs));
    long long int eip = pgregs[REG_RIP];
    #elif defined(__aarch64)
    ucontext_t* ptrUC = (ucontext_t*)ptr;
		long long int eip = ptrUC->uc_mcontext.arm_pc;
    #endif
    if (eip != (long long int)addresses[2])
      printf("Catched when running code at %llx\n", (long long int)addresses[2]);
    printf("EIP catched at %llX\n", eip);
  }
  else printf("signal[%d] catched when running code at unknown address\n", signum);

  abort();
}


