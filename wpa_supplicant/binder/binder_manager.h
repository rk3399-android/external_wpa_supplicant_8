/*
 * binder interface for wpa_supplicant daemon
 * Copyright (c) 2004-2016, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004-2016, Roshan Pius <rpius@google.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_SUPPLICANT_BINDER_BINDER_MANAGER_H
#define WPA_SUPPLICANT_BINDER_BINDER_MANAGER_H

#include <map>
#include <string>

#include "fi/w1/wpa_supplicant/IIfaceCallback.h"
#include "fi/w1/wpa_supplicant/INetworkCallback.h"
#include "fi/w1/wpa_supplicant/ISupplicantCallback.h"

#include "iface.h"
#include "network.h"
#include "supplicant.h"

struct wpa_global;
struct wpa_supplicant;

namespace wpa_supplicant_binder {

/**
 * BinderManager is responsible for managing the lifetime of all
 * binder objects created by wpa_supplicant. This is a singleton
 * class which is created by the supplicant core and can be used
 * to get references to the binder objects.
 */
class BinderManager
{
public:
	static BinderManager *getInstance();
	static void destroyInstance();

	// Methods called from wpa_supplicant core.
	int registerBinderService(struct wpa_global *global);
	int registerInterface(struct wpa_supplicant *wpa_s);
	int unregisterInterface(struct wpa_supplicant *wpa_s);
	int
	registerNetwork(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid);
	int
	unregisterNetwork(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid);

	// Methods called from binder objects.
	int getIfaceBinderObjectByIfname(
	    const std::string &ifname,
	    android::sp<fi::w1::wpa_supplicant::IIface> *iface_object);
	int getNetworkBinderObjectByIfnameAndNetworkId(
	    const std::string &ifname, int network_id,
	    android::sp<fi::w1::wpa_supplicant::INetwork> *network_object);
	int addSupplicantCallbackBinderObject(
	    const android::sp<fi::w1::wpa_supplicant::ISupplicantCallback>
		&callback);
	int addIfaceCallbackBinderObject(
	    const std::string &ifname,
	    const android::sp<fi::w1::wpa_supplicant::IIfaceCallback>
		&callback);
	int addNetworkCallbackBinderObject(
	    const std::string &ifname, int network_id,
	    const android::sp<fi::w1::wpa_supplicant::INetworkCallback>
		&callback);

private:
	BinderManager() = default;
	~BinderManager() = default;
	BinderManager(const BinderManager &) = default;
	BinderManager &operator=(const BinderManager &) = default;

	const std::string
	getNetworkObjectMapKey(const std::string &ifname, int network_id);

	// Singleton instance of this class.
	static BinderManager *instance_;
	// The main binder service object.
	android::sp<Supplicant> supplicant_object_;
	// Map of all the interface specific binder objects controlled by
	// wpa_supplicant. This map is keyed in by the corresponding
	// |ifname|.
	std::map<const std::string, android::sp<Iface>> iface_object_map_;
	// Map of all the network specific binder objects controlled by
	// wpa_supplicant. This map is keyed in by the corresponding
	// |ifname| & |network_id|.
	std::map<const std::string, android::sp<Network>> network_object_map_;
};

} // namespace wpa_supplicant_binder

#endif // WPA_SUPPLICANT_BINDER_BINDER_MANAGER_H
