/*-------------------------------------------------------------
   MOUSDEMO.C -- Demonstrates mouse message processing.
                 (c) 1988, Ziff Communications Co.
                 PC Magazine * Charles Petzold, 8/88 and 11/88
  -------------------------------------------------------------*/

#define INCL_WIN
#define INCL_GPI
#include <os2.h>
#include <stdio.h>
#define ID_TIMER    1

MRESULT EXPENTRY ClientWndProc (HWND, USHORT, MPARAM, MPARAM) ;
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

     WinRegisterClass (hab, szClientClass, ClientWndProc, CS_SIZEREDRAW, 0) ;

     hwndFrame = WinCreateStdWindow (HWND_DESKTOP, WS_VISIBLE,
                                     &flFrameFlags, szClientClass, NULL,
                                     0L, NULL, 0, &hwndClient) ;

     WinSendMsg (hwndFrame, WM_SETICON,
                 WinQuerySysPointer (HWND_DESKTOP, SPTR_APPICON, FALSE),
                 NULL) ;

     while (WinGetMsg (hab, &qmsg, NULL, 0, 0))
          WinDispatchMsg (hab, &qmsg) ;

     WinDestroyWindow (hwndFrame) ;
     WinDestroyMsgQueue (hmq) ;
     WinTerminate (hab) ;
     return 0 ;
     }

VOID PaintWindow (HWND hwnd, HPS hps, SHORT xPointer, SHORT yPointer)
     {
     static CHAR szBuffer [40] ;
     RECTL       rcl ;

     sprintf (szBuffer, "    Pointer Position = (%d, %d)    ",
              xPointer, yPointer) ;
     WinQueryWindowRect (hwnd, &rcl) ;
     GpiSetBackMix (hps, BM_OVERPAINT) ;
     WinDrawText (hps, -1, szBuffer, &rcl, CLR_NEUTRAL, CLR_BACKGROUND,
                  DT_CENTER | DT_VCENTER) ;
     }

MRESULT EXPENTRY ClientWndProc (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
     {
     static SHORT xPointer, yPointer ;
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
               xPointer = (SHORT) ptl.x ;
               yPointer = (SHORT) ptl.y ;

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
               return 1 ;

          case WM_BUTTON1DBLCLK:
               DosBeep (1024, 100) ;
               return 1 ;

          case WM_PAINT:
               hps = WinBeginPaint (hwnd, NULL, NULL) ;
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
