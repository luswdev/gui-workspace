/**
 *******************************************************************************
 * @file       hook.c
 * @version    V1.1.6    
 * @date       2014.05.23
 * @brief      hook management implementation code of CooCox CoOS kernel.	
 *******************************************************************************
 * @copy
 *
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  
 *      * Redistributions of source code must retain the above copyright 
 *  notice, this list of conditions and the following disclaimer. 
 *      * Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution. 
 *      * Neither the name of the <ORGANIZATION> nor the names of its 
 *  contributors may be used to endorse or promote products derived 
 *  from this software without specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 *  THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * <h2><center>&copy; COPYRIGHT 2014 CooCox </center></h2>
 *******************************************************************************
 */ 


/*---------------------------- Inlcude --------------------------------------*/
#include <coocox.h>
/*#include <stdio.h>

#define RED_BOLD "\x1b[;31;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"*/

/**
 *******************************************************************************
 * @brief      IDLE task of OS	 
 * @param[in]  pdata	The parameter passed to IDLE task.		 
 * @param[out] None 
 * @retval     None	 
 *
 * @par Description
 * @details    This function is system IDLE task code.	 
 *******************************************************************************
 */
void CoIdleTask(void* pdata)
{
	/*extern OSTCB    TCBTbl[CFG_MAX_USER_TASKS+SYS_TASK_NUM];
	P_OSTCB ptcb;
	int i;
    
    printf("[%10s] Enter idle\r\n", "system");
	ptcb = &TCBTbl[0];
	for ( i = 0; i < (CFG_MAX_USER_TASKS+SYS_TASK_NUM-1); i++ ) {
		printf("Task %2d: state =>", ptcb->taskID);
		switch (ptcb->state) {
			case TASK_READY:
				printf(GRN_BOLD" TASK_READY   "RESET);
				break;
		
			case TASK_RUNNING:
				printf(BLU_BOLD" TASK_RUNNING "RESET);
				break;
			
			case TASK_WAITING:
				printf(YEL_BOLD" TASK_WAITING "RESET);
				break;
			
			case TASK_DORMANT:
				printf(RED_BOLD" TASK_DORMANT "RESET);
				break;
		}
		
		printf(", priority => %d\r\n", ptcb->prio);
		ptcb++;
	}
	
	printf("[%10s] Sleeping...\r\n", "system");*/
	for( ; ; )
		;
}


/**
 *******************************************************************************
 * @brief      Hook for stack overflow	 
 * @param[in]  taskID	Piont to the task which lead to stack overflow.		 
 * @param[out] None 
 * @retval     None	 
 *
 * @par Description
 * @details    This function is a hook for stack overflow.	 
 *******************************************************************************
 */
void CoStkOverflowHook(OS_TID taskID)
{
    /* Process stack overflow  here */
    for(; ;) 
    {
      
    }
}






