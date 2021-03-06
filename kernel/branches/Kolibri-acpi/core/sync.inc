;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                              ;;
;; Copyright (C) KolibriOS team 2004-2015. All rights reserved. ;;
;; Distributed under terms of the GNU General Public License    ;;
;;                                                              ;;
;; Synhronization for MenuetOS.                                 ;;
;; Author: Halyavin Andrey, halyavin@land.ru                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

$Revision$

align 4
;struct futex*  __fastcall create_futex(int *ptr)
create_futex:
        push    ecx
        mov     ecx, sizeof.FUTEX
        call    create_object
        pop     ecx
        test    eax, eax
        jz .fail

        mov     [eax+FUTEX.magic], 'FUTX'
        mov     [eax+FUTEX.destroy], 0
        mov     [eax+FUTEX.pointer], ecx
        lea     ecx, [eax+FUTEX.wait_list]
        list_init ecx
        mov     [eax+FUTEX.flags], 0
.fail:
        ret

align 4
;int __fastcall destroy_futex(struct futex *futex)
destroy_futex:
        push    esi
        mov     esi, [current_process]
        mov     edx, [ecx+FUTEX.handle]

        pushfd
        cli

        lea     eax, [ecx+FUTEX.wait_list]
        cmp     eax, [eax+LHEAD.next]
        jne     .fail

        mov     eax, [esi+PROC.ht_next]
        mov     [esi+PROC.htab+edx*4], eax
        mov     [esi+PROC.ht_next], edx
        inc     [esi+PROC.ht_free]

        popfd
        pop     esi

        mov     eax, ecx
        call    free
        xor     eax, eax
        ret

.fail:
        popfd
        pop     esi
        mov     eax, -1
        ret


iglobal
align 4
f77call:
        dd f77.futex_init     ;0
        dd f77.futex_destroy  ;1
        dd f77.futex_wait     ;2
        dd f77.futex_wake     ;3
.end:
endg

align 4
sys_synchronization:
f77:
        test    ebx, ebx
        jz      .futex_init

        cmp     ebx, (f77call.end-f77call)/4
        jae     .fail

        cmp     ecx, STDERR_FILENO
        jbe     .fail
        cmp     ecx, (PROC.pdt_0 - PROC.htab)/4
        jae     .fail

        mov     edi, [current_process]
        mov     ebp, [edi+PROC.htab+ecx*4]

        cmp     [ebp+FUTEX.magic], 'FUTX'
        jne     .fail
        cmp     [ebp+FUTEX.handle], ecx
        jne     .fail

        jmp     dword [f77call+ebx*4]

.fail:
        mov     [esp+SYSCALL_STACK._eax], -1
        ret

align 4
.futex_init:
        call    create_futex
        test    eax, eax
        jz      @F
        mov     eax, [eax+FUTEX.handle]
@@:
        mov     [esp+SYSCALL_STACK._eax], eax
        ret


align 4
;ecx futex handle
;edi current process
;ebp futex object
.futex_destroy:
        mov     ecx, ebp
        call    destroy_futex
        mov     [esp+SYSCALL_STACK._eax], eax
        ret

align 4
;ecx futex handle
;edx control value
;esi timeout
;edi current process
;ebp futex object
.futex_wait:
        test    esi, esi
        jnz     .futex_wait_timeout
        mov     ecx, [ebp+FUTEX.pointer]
        mov     eax, edx
        lock cmpxchg [ecx], edx
        je      .wait_slow

        mov     [esp+SYSCALL_STACK._eax], -2
        ret

.wait_slow:
        pushfd
        cli

        sub     esp, sizeof.MUTEX_WAITER
        mov     ebx, [TASK_BASE]
        mov     [esp+MUTEX_WAITER.task], ebx
        lea     esi, [ebp+FUTEX.wait_list]

        list_add_tail esp, esi      ;esp= new waiter, esi= list head
        mov     eax, edx
.again:
        mov     [ebx+TASKDATA.state], 1
        call    change_task

        lock cmpxchg [ecx], edx
        je      .again

        list_del esp
        add     esp, sizeof.MUTEX_WAITER

        popfd
        mov     [esp+SYSCALL_STACK._eax], 0
        ret

align 4
;ecx futex handle
;edx control value
;esi timeout
;edi current process
;ebp futex object

.futex_wait_timeout:
        mov     ecx, [ebp+FUTEX.pointer]
        mov     eax, edx
        lock cmpxchg [ecx], edx         ;wait until old_value == new_value
        je      .wait_slow_timeout

        mov     [esp+SYSCALL_STACK._eax], -2
        ret

align 4
.wait_test:
        xor     eax, eax
        ret

.wait_slow_timeout:
        pushfd
        cli

        sub     esp, sizeof.MUTEX_WAITER

        mov     ebx, [current_slot]
        mov     [ebx+APPDATA.wait_test], f77.wait_test
        mov     [ebx+APPDATA.wait_timeout], esi
        mov     [ebx+APPDATA.wait_param], ebp
        mov     eax, [timer_ticks]
        mov     [ebx+APPDATA.wait_begin], eax
        mov     eax, [TASK_BASE]
        mov     [eax+TASKDATA.state], 5

        mov     [esp+MUTEX_WAITER.task], eax
        lea     esi, [ebp+FUTEX.wait_list]

        list_add_tail esp, esi      ;esp= new waiter, esi= list head

.again_timeout:
        call    change_task
        mov     eax, [ebx+APPDATA.wait_param]
        test    eax, eax
        jz      .timeout

        mov     eax, edx
        lock cmpxchg [ecx], edx
        jz .again_timeout
@@:
        list_del esp
        add     esp, sizeof.MUTEX_WAITER

        popfd
        mov     [esp+SYSCALL_STACK._eax], 0
        ret

.timeout:
        list_del esp
        add     esp, sizeof.MUTEX_WAITER

        popfd
        mov     [esp+SYSCALL_STACK._eax], -1
        ret


align 4
;ecx futex handle
;edx number of threads
;edi current process
;ebp futex object
.futex_wake:

        xor     ecx, ecx

        pushfd
        cli

        lea     ebx, [ebp+FUTEX.wait_list]
        mov     esi, [ebx+LHEAD.next]
.wake:
        cmp     esi, ebx
        je      .done

        mov     eax, [esi+MUTEX_WAITER.task]
        mov     [eax+TASKDATA.state], 0

        mov     esi, [esi+MUTEX_WAITER.list.next]
        inc     ecx
        cmp     ecx, edx
        jb      .wake
.done:
        popfd
        mov     [esp+SYSCALL_STACK._eax], ecx
        ret

RWSEM_WAITING_FOR_WRITE equ 0
RWSEM_WAITING_FOR_READ  equ 1

;void  __fastcall mutex_init(struct mutex *lock)

align 4
mutex_init:
        mov     [ecx+MUTEX.wait_list.next], ecx
        mov     [ecx+MUTEX.wait_list.prev], ecx
        mov     [ecx+MUTEX.count], 1
        ret

;void  __fastcall mutex_lock(struct mutex *lock)

align 4
mutex_lock:

        dec     [ecx+MUTEX.count]
        jns     .done

        pushfd
        cli

        sub     esp, sizeof.MUTEX_WAITER

        list_add_tail esp, ecx      ;esp= new waiter, ecx= list head

        mov     edx, [TASK_BASE]
        mov     [esp+MUTEX_WAITER.task], edx

.forever:

        mov     eax, -1
        xchg    eax, [ecx+MUTEX.count]
        dec     eax
        jz      @F

        mov     [edx+TASKDATA.state], 1
        call    change_task
        jmp     .forever
@@:
        mov     eax, ecx
        list_del esp

        cmp     [eax+MUTEX.wait_list.next], eax
        jne     @F

        mov     [eax+MUTEX.count], 0
@@:
        add     esp, sizeof.MUTEX_WAITER

        popfd
.done:
        ret

;void  __fastcall mutex_unlock(struct mutex *lock)

align 4
mutex_unlock:

        pushfd
        cli

        mov     eax, [ecx+MUTEX.wait_list.next]
        cmp     eax, ecx
        mov     [ecx+MUTEX.count], 1
        je      @F

        mov     eax, [eax+MUTEX_WAITER.task]
        mov     [eax+TASKDATA.state], 0
@@:
        popfd
        ret


;void __fastcall init_rwsem(struct rw_semaphore *sem)

align 4
init_rwsem:
        mov     [ecx+RWSEM.wait_list.next], ecx
        mov     [ecx+RWSEM.wait_list.prev], ecx
        mov     [ecx+RWSEM.count], 0
        ret

;void __fastcall down_read(struct rw_semaphore *sem)

align 4
down_read:
        pushfd
        cli

        mov     eax, [ecx+RWSEM.count]
        test    eax, eax
        js      @F

        cmp     ecx, [ecx+RWSEM.wait_list.next]
        je      .ok
@@:
        sub     esp, sizeof.MUTEX_WAITER

        mov     eax, [TASK_BASE]
        mov     [esp+MUTEX_WAITER.task], eax
        mov     [esp+MUTEX_WAITER.type], RWSEM_WAITING_FOR_READ
        mov     [eax+TASKDATA.state], 1

        list_add_tail esp, ecx      ;esp= new waiter, ecx= list head

        call    change_task

        add     esp, sizeof.MUTEX_WAITER
        popfd
        ret
.ok:
        inc     eax
        mov     [ecx+RWSEM.count], eax

        popfd
        ret

;void __fastcall down_write(struct rw_semaphore *sem)

align 4
down_write:
        pushfd
        cli
        sub     esp, sizeof.MUTEX_WAITER

        mov     edx, [TASK_BASE]
        mov     [esp+MUTEX_WAITER.task], edx
        mov     [esp+MUTEX_WAITER.type], RWSEM_WAITING_FOR_WRITE
        mov     [edx+TASKDATA.state], 1

        list_add_tail esp, ecx      ;esp= new waiter, ecx= list head

        xor     eax, eax
        not     eax

.forever:
        test    eax, [ecx+RWSEM.count]
        jz      @F

        mov     [edx+TASKDATA.state], 1
        call    change_task
        jmp     .forever
@@:
        mov     [ecx+RWSEM.count], eax
        list_del esp

        add     esp, sizeof.MUTEX_WAITER
        popfd
        ret

;void __fastcall up_read(struct rw_semaphore *sem)

align 4
up_read:
        pushfd
        cli

        dec     [ecx+RWSEM.count]
        jnz     @F

        mov     eax, [ecx+RWSEM.wait_list.next]
        cmp     eax, ecx
        je      @F

        mov     eax, [eax+MUTEX_WAITER.task]
        mov     [eax+TASKDATA.state], 0
@@:
        popfd
        ret

;void __fastcall up_write(struct rw_semaphore *sem)

align 4
up_write:

        pushfd
        cli

        mov     eax, [ecx+RWSEM.wait_list.next]
        mov     [ecx+RWSEM.count], 0

        cmp     ecx, eax
        je      .done

        mov     edx, [eax+MUTEX_WAITER.type]
        test    edx, edx
        jnz     .wake

        mov     eax, [eax+MUTEX_WAITER.task]
        mov     [eax+TASKDATA.state], 0
.done:
        popfd
        ret

.wake:
        push    ebx
        push    esi
        push    edi

        xor     esi, esi
        mov     edi, ecx

.wake_list:

        mov     ebx, [eax+MUTEX_WAITER.list.next]
        list_del eax
        mov     edx, [eax+MUTEX_WAITER.task]
        mov     [edx+TASKDATA.state], 0
        inc     esi
        cmp     edi, ebx
        je      .wake_done

        mov     ecx, [ebx+MUTEX_WAITER.type]
        test    ecx, ecx
        jz      .wake_done

        mov     eax, ebx
        jmp     .wake_list

.wake_done:
        add     [edi+RWSEM.count], esi

        pop     edi
        pop     esi
        pop     ebx
        popfd
        ret


purge RWSEM_WAITING_FOR_WRITE
purge RWSEM_WAITING_FOR_READ


if ~defined sync_inc
sync_inc_fix:
sync_inc fix sync_inc_fix

;simplest mutex.
macro SimpleMutex name
{
;  iglobal
    name dd 0
    name#.type = 1
;  endg
}
macro WaitSimpleMutex name
{
  local start_wait,ok
start_wait=$
        cli
        cmp     [name], dword 0
        jz      ok
        sti
        call    change_task
        jmp     start_wait
ok=$
        push    eax
        mov     eax, dword [TASK_BASE+second_base_address]
        mov     eax, [eax+TASKDATA.pid]
        mov     [name], eax
        pop     eax
        sti
}
macro ReleaseSimpleMutex name
{
        mov     [name], dword 0
}
macro TryWaitSimpleMutex name  ;result in eax and in flags
{
  local ok,try_end
        cmp     [name], dword 0
        jz      ok
        xor     eax, eax
        jmp     try_end
ok=$
        xor     eax, eax
        inc     eax
try_end=$
}
macro SimpleCriticalSection name
{
;  iglobal
    name  dd 0
          dd 0
    name#.type=2
;  endg
}
macro WaitSimpleCriticalSection name
{
  local start_wait,first_wait,inc_counter,end_wait
        push    eax
        mov     eax, [TASK_BASE+second_base_address]
        mov     eax, [eax+TASKDATA.pid]
start_wait=$
        cli
        cmp     [name], dword 0
        jz      first_wait
        cmp     [name], eax
        jz      inc_counter
        sti
        call    change_task
        jmp     start_wait
first_wait=$
        mov     [name], eax
        mov     [name+4], dword 1
        jmp     end_wait
inc_counter=$
        inc     dword [name+4]
end_wait=$
        sti
        pop     eax
}
macro ReleaseSimpleCriticalSection name
{
  local release_end
        dec     dword [name+4]
        jnz     release_end
        mov     [name], dword 0
release_end=$
}
macro TryWaitSimpleCriticalSection name ;result in eax and in flags
{
  local ok,try_end
        mov     eax, [CURRENT_TASK+second_base_address]
        mov     eax, [eax+TASKDATA.pid]
        cmp     [name], eax
        jz      ok
        cmp     [name], 0
        jz      ok
        xor     eax, eax
        jmp     try_end
ok=$
        xor     eax, eax
        inc     eax
try_end=$
}
_cli equ call MEM_HeapLock
_sti equ call MEM_HeapUnLock
end if

