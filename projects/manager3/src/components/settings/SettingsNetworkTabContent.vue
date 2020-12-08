<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.network.server.title">
        <template v-slot:content>
          <WrappedTextInput
            label="settings.network.server.hostname"
            getter="system/hostname"
            setter="system/post"
            apiKey="hostname"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.network.wifi.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.network.wifi.toggleButtonLabel"
            getter="wifi/enabled"
            setter="wifi/post"
            apiKey="enabled"
          />
          <p class="help">{{ $t('settings.network.wifi.paragraphe_1') }}</p>

          <q-splitter
            v-model="splitterModel"
          >

            <template v-slot:before>
              <q-tabs
                active-color="primary"
                class="text-grey"
                indicator-color="accent"
                inline-label
                v-model="tab"
                vertical
              >
                <q-tab :label="$t('settings.network.wifi.wifi_1')" icon="mdi-numeric-1-circle" name="wifi_1"/>
                <q-tab :label="$t('settings.network.wifi.wifi_2')" icon="mdi-numeric-2-circle" name="wifi_2"/>
                <q-tab :label="$t('settings.network.wifi.wifi_3')" icon="mdi-numeric-3-circle" name="wifi_3"/>
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
                    getter="wifi/ssid"
                    setter="wifi/post"
                    apiKey="ssid"
                  />
                  <WrappedTextInput
                    label="settings.network.wifi.key"
                    getter="wifi/key"
                    setter="wifi/post"
                    apiKey="key"
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

<script>
  import WrappedTextInput from 'components/global/WrappedTextInput'
  import WrappedToggle from 'components/global/WrappedToggle'
  import FormFragmentContainer from '../global/FormFragmentContainer'

  export default {
    name: 'SettingsNetworkTabContent',
    components: {
      WrappedTextInput,
      FormFragmentContainer,
      WrappedToggle,
    },
    created() {
      this.$store.dispatch('system/get')
      this.$store.dispatch('wifi/get')
    },
    data() {
      return {
        text: '',
        tab: 'wifi_1',
        splitterModel: 20,
      }
    },
  }
</script>

<style lang="sass" scoped>

</style>
