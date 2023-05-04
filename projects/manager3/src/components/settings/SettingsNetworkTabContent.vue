<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.network.server.title">
        <template v-slot:content>
          <WrappedTextInput
            label="settings.network.server.hostname"
            :getter="system.hostname"
            :setter="systemStore.post"
            apiKey="hostname"
            v-if="system.hostname"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.network.wifi.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.network.wifi.toggleButtonLabel"
            :getter="wifi.enabled"
            :setter="wifiStore.post"
            apiKey="enabled"
            v-if="wifi.enabled"
            help
          >
            <template v-slot:help>
              {{ $t('settings.network.wifi.help') }}
            </template>
          </WrappedToggle>

          <q-separator/>

          <WrappedSelect
            label="settings.network.wifi.region.title"
            :options="regionOptions"
            :getter="wifi.region"
            :setter="wifiStore.post"
            apiKey="region"
            v-if="wifi.region"
            help
          >
            <template v-slot:help>
              {{ $t('settings.network.wifi.region.help') }}
            </template>
          </WrappedSelect>

          <q-separator/>

          <WrappedTextInput
            label="settings.network.wifi.ip.title"
            :getter="wifi.ip"
            :setter="wifiStore.post"
            apiKey="ip"
            v-if="wifi.ip"
          />

          <WrappedTextInput
            label="settings.network.wifi.netmask.title"
            :getter="wifi.netmask"
            :setter="wifiStore.post"
            apiKey="netmask"
            v-if="wifi.netmask"
          />

          <WrappedTextInput
            label="settings.network.wifi.gateway.title"
            :getter="wifi.gateway"
            :setter="wifiStore.post"
            apiKey="gateway"
            v-if="wifi.gateway"
          />

          <q-separator/>

          <q-splitter v-model="splitterModel">
            <template v-slot:before>
              <q-tabs
                active-color="primary"
                class="text-grey"
                indicator-color="accent"
                inline-label
                v-model="tab"
                vertical
              >
                <q-tab
                  icon="mdi-numeric-1-circle"
                  name="wifi_1"
                />
                <q-tab
                  icon="mdi-numeric-2-circle"
                  name="wifi_2"
                  disable
                />
                <q-tab
                  icon="mdi-numeric-3-circle"
                  name="wifi_3"
                  disable
                />
              </q-tabs>
            </template>

            <template v-slot:after>
              <q-tab-panels
                animated
                swipeable
                transition-next="jump-up"
                transition-prev="jump-up"
                v-model="tab"
                vertical
              >
                <q-tab-panel name="wifi_1">
                  <WrappedTextInput
                    label="settings.network.wifi.ssid"
                    :getter="wifi.ssid"
                    :setter="wifiStore.post"
                    apiKey="ssid"
                    v-if="wifi.ssid"
                  />
                  <WrappedTextInput
                    label="settings.network.wifi.key"
                    :getter="wifi.key"
                    :setter="wifiStore.post"
                    apiKey="key"
                    v-if="wifi.key"
                    password
                  />
                </q-tab-panel>

                <q-tab-panel name="wifi_2">
                  <q-input
                    :label="$t('settings.network.wifi.ssid')"
                    class="q-mb-md"
                    dense
                    standout="bg-primary text-white"
                    v-model="text"
                  />
                  <q-input
                    :label="$t('settings.network.wifi.key')"
                    dense
                    standout="bg-primary text-white"
                    v-model="text"
                  />
                </q-tab-panel>

                <q-tab-panel name="wifi_3">
                  <q-input
                    :label="$t('settings.network.wifi.ssid')"
                    class="q-mb-md"
                    dense
                    standout="bg-primary text-white"
                    v-model="text"
                  />
                  <q-input
                    :label="$t('settings.network.wifi.key')"
                    dense
                    standout="bg-primary text-white"
                    v-model="text"
                  />
                </q-tab-panel>
              </q-tab-panels>
            </template>

          </q-splitter>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import { useSystemStore } from 'stores/configuration/system';
import { useWifiStore } from 'stores/configuration/wifi';
import { ref } from 'vue';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';

const systemStore = useSystemStore();
systemStore.fetch();
const { system } = storeToRefs(systemStore);

const wifiStore = useWifiStore();
wifiStore.fetch();
const { wifi, regionOptions } = storeToRefs(wifiStore);

const text = ref<string>('');
const tab = ref<string>('wifi_1');
const splitterModel = ref<number>(20);
</script>
