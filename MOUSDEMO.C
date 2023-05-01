/*-------------------------------------------------------------
   MOUSDEMO.C -- Demonstrates mouse message processing.
  -------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#define ID_TIMER    1

MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM) ;
HAB  hab ;

int main (void)
     {
     static CHAR  szClientClass [] = "MousDemo" ;
     static ULONG flFrameFlags = FCF_TITLEBAR      | FCF_SYSMENU |
                                 FCF_SIZEBORDER    | FCF_MINMAX  |
                                 FCF_SHELLPOSITION | FCF_TASKLIST ;
     HMQ          hmq ;
     HWND         hwndFrame, hwndClient ;
     QMSG         qmsg ;

     hab = WinInitialize (0) ;
     hmq = WinCreateMsgQueue (hab, 0) ;

     WinRegisterClass (hab, (PCSZ) szClientClass, (PFNWP) ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, (PCSZ) szClientClass, NULLHANDLE,
                                     0L, NULLHANDLE, 0, &hwndClient) ;

     WinSendMsg (hwndFrame, WM_SETICON,
                 (MPARAM) WinQuerySysPointer (HWND_DESKTOP, SPTR_APPICON, FALSE),
                 NULLHANDLE) ;

     while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

VOID PaintWindow (HWND hwnd, HPS hps, LONG xPointer, LONG yPointer)
     {
     static CHAR szBuffer [40] ;
     RECTL       rcl ;

     sprintf (szBuffer, "Pointer Position = (%d, %d)",
              (int) xPointer, (int) yPointer) ;
     WinQueryWindowRect (hwnd, &rcl) ;
     GpiSetBackMix (hps, BM_OVERPAINT) ;
     WinDrawText (hps, -1, (PCCH) szBuffer, &rcl, CLR_NEUTRAL, CLR_BACKGROUND,
                  DT_CENTER | DT_VCENTER) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
     {
     static LONG xPointer, yPointer ;
     HPS          hps;
     POINTL       ptl ;

     switch (msg)
	  {
          case WM_CREATE:
               WinStartTimer (hab, hwnd, ID_TIMER, 1000) ;
               return 0 ;

          case WM_TIMER:
               WinQueryPointerPos (HWND_DESKTOP, &ptl) ;
               WinMapWindowPoints (HWND_DESKTOP, hwnd, &ptl, 1) ;
               xPointer = (LONG) ptl.x ;
               yPointer = (LONG) ptl.y ;

               hps = WinGetPS (hwnd) ;
               PaintWindow (hwnd, hps, xPointer, yPointer) ;
               WinReleasePS (hps) ;
               return 0 ;

          case WM_MOUSEMOVE:
               xPointer = MOUSEMSG(&msg)->x ;
               yPointer = MOUSEMSG(&msg)->y ;

               hps = WinGetPS (hwnd) ;
               PaintWindow (hwnd, hps, xPointer, yPointer) ;
               WinReleasePS (hps) ;
               break ;

          case WM_BUTTON1DOWN:
               DosBeep (256, 100) ;
               break ;

          case WM_BUTTON1UP:
               DosBeep (512, 100) ;
               return ((MRESULT)TRUE) ;

          case WM_BUTTON1DBLCLK:
               DosBeep (1024, 100) ;
               return ((MRESULT)TRUE) ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULLHANDLE, NULLHANDLE) ;
               GpiErase (hps) ;
               PaintWindow (hwnd, hps, xPointer, yPointer) ;
               WinEndPaint (hps) ;
               return 0 ;

          case WM_DESTROY:
               WinStopTimer (hab, hwnd, ID_TIMER) ;
               return 0 ;
          }
     return WinDefWindowProc (hwnd, msg, mp1, mp2) ;
     }
