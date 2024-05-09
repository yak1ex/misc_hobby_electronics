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
teams_mute := 0

IsZoomMeetingExist()
{
  ; Meeting window or Control window for screen share
  return WinExist("ahk_class ZPContentViewWndClass") || WinExist("ahk_class ZPFloatToolbarClass")
}

IsTeamsMeetingExist()
{
  list := WinGetList("ahk_class TeamsWebView")
  return (list.Length > 1)
}

ToggleTeamsMute()
{
  if (IsTeamsMeetingExist())
  {
    active := WinGetID("A")
    WinActivate "ahk_class TeamsWebView"
    Send "+^m"
    Sleep 1
    WinActivate active
  }
}

; For Zoom: Assuming global shortcut for Alt+A is enabled
;           we don't need activate Zoom window
#HotIf IsZoomMeetingExist()
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

; For Teams: Need to activate Teams window
#HotIf IsTeamsMeetingExist()
F24::
{
  global teams_mute
  if teams_mute == 0 {
    ToggleTeamsMute()
    teams_mute := 1
  }
}
F24 Up::
{
  global teams_mute
  ToggleTeamsMute()
  teams_mute := 0
}
F23::ToggleTeamsMute()
