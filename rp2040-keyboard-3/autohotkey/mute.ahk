; mute.ahk
;
; SPDX-License-Identifier: 0BSD
;
; mute switcher for external keyboard
; F24 is assigned for push-to-talk
; F23 is assigned for toggle to mute

CRITERIA_ZOOMCLASS1 := "ahk_class ZPContentViewWndClass"
CRITERIA_ZOOMCLASS2 := "ahk_class ZPFloatToolbarClass"
CRITERIA_TEMASCLASS := "ahk_class TeamsWebView"
CRITERIA_GMEETCLASS := "Meet ahk_exe chrome.exe ahk_class Chrome_WidgetWin_1"

; Space is assigned to a push-to-talk function for zoom
; but it is required to activate a zoom window,
; so toggle when each of down/up event is triggered.

zoom_mute := 0
teams_mute := 0
gmeet_mute := 0

IsZoomMeetingExist()
{
  ; Meeting window or Control window for screen share
  return WinExist(CRITERIA_ZOOMCLASS1) || WinExist(CRITERIA_ZOOMCLASS2)
}

IsTeamsMeetingExist()
{
  list := WinGetList(CRITERIA_TEMASCLASS)
  return (list.Length > 1)
}

ToggleTeamsMute()
{
  if (IsTeamsMeetingExist())
  {
    active := WinGetID("A")
    WinActivate CRITERIA_TEMASCLASS
    Send "+^m"
    Sleep 1
    WinActivate active
  }
}

IsGMeetMeetingExist()
{
  list := WinGetList(CRITERIA_GMEETCLASS)
  return (list.Length > 0)
}

ToggleGMeetMute()
{
  if (IsGMeetMeetingExist())
  {
    active := WinGetID("A")
    WinActivate CRITERIA_GMEETCLASS
    Send "^d"
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
    Send "!a"
    zoom_mute := 1
  }
}
F24 Up::
{
  global zoom_mute
  Send "!a"
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

; For Google Meet: Need to activate Chrome window
#HotIf IsGMeetMeetingExist()
F24::
{
  global gmeet_mute
  if gmeet_mute == 0 {
    ToggleGMeetMute()
    gmeet_mute := 1
  }
}
F24 Up::
{
  global gmeet_mute
  ToggleGMeetMute()
  gmeet_mute := 0
}
F23::ToggleGMeetMute()
