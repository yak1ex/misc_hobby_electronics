; mute.ahk
;
; SPDX-License-Identifier: 0BSD
;
; mute switcher for external keyboard
; F24 is assigned for push-to-talk
; F23 is assigned for toggle to mute

; Space is assigned to a push-to-talk function for zoom
; but it is required to activate a zoom window,
; so toggle when each of down/up event is triggered.

zoom_mute := 0

IsZoomExist()
{
  ; Meeting window or Control window for screen share
  return WinExist("ahk_class ZPContentViewWndClass") || WinExist("ahk_class ZPFloatToolbarClass")
}

; For Zoom: Need to enable global shortcut for Alt+A
#HotIf IsZoomExist()
F24::
{
  global zoom_mute
  if zoom_mute == 0 {
    Send "!{a}"
    zoom_mute := 1
  }
}
F24 Up::
{
  global zoom_mute
  Send "!{a}"
  zoom_mute := 0
}
F23::!a
