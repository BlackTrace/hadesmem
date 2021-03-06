// Copyright (C) 2010-2015 Joshua Boyce
// See the file COPYING for copying permission.

#pragma once

#include <cstdint>

#include <windows.h>
#include <hadesmem/detail/warning_disable_prefix.hpp>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <hadesmem/detail/warning_disable_suffix.hpp>

#include <hadesmem/config.hpp>

namespace hadesmem
{
namespace cerberus
{
class DirectInput8WProxy : public IDirectInput8W
{
public:
  explicit DirectInput8WProxy(IDirectInput8W* direct_input)
    : direct_input_{direct_input}
  {
  }

  // IUnknown
  HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* obj) override;
  ULONG WINAPI AddRef() override;
  ULONG WINAPI Release() override;

  // IDirectInput8W
  HRESULT WINAPI CreateDevice(REFGUID rguid,
                              LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice,
                              LPUNKNOWN pUnkOuter) override;
  HRESULT WINAPI EnumDevices(DWORD dwDevType,
                             LPDIENUMDEVICESCALLBACKW lpCallback,
                             LPVOID pvRef,
                             DWORD dwFlags) override;
  HRESULT WINAPI GetDeviceStatus(REFGUID rguidInstance) override;
  HRESULT WINAPI RunControlPanel(HWND hwndOwner, DWORD dwFlags) override;
  HRESULT WINAPI Initialize(HINSTANCE hinst, DWORD dwVersion) override;
  HRESULT WINAPI FindDevice(REFGUID rguidClass,
                            LPCTSTR ptszName,
                            LPGUID pguidInstance) override;
  HRESULT WINAPI EnumDevicesBySemantics(LPCTSTR ptszUserName,
                                        LPDIACTIONFORMAT lpdiActionFormat,
                                        LPDIENUMDEVICESBYSEMANTICSCB lpCallback,
                                        LPVOID pvRef,
                                        DWORD dwFlags) override;
  HRESULT WINAPI ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback,
                                  LPDICONFIGUREDEVICESPARAMS lpdiCDParams,
                                  DWORD dwFlags,
                                  LPVOID pvRefData) override;

private:
  void Cleanup();

  std::int64_t refs_{1};
  IDirectInput8W* direct_input_{};
};
}
}
