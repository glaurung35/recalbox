<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.system.internationalization.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.system.internationalization.langSelect.label"
            :options="languageOptions"
            :getter="system.language"
            :setter="systemStore.post"
            apiKey="language"
            v-if="system.language"
          />
          <WrappedSelect
            label="settings.system.internationalization.keyboardSelect.label"
            :options="kblayoutOptions"
            :getter="system.kblayout"
            :setter="systemStore.post"
            apiKey="kblayout"
            v-if="system.kblayout"
          />
          <WrappedSelect
            label="settings.system.internationalization.timeZoneSelect.label"
            :options="timezoneOptions"
            :getter="system.timezone"
            :setter="systemStore.post"
            apiKey="timezone"
            v-if="system.timezone"
          />
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.splashScreen.title">
        <template v-slot:content>
          <WrappedSlider
            label="settings.system.splashScreen.splashLength.label"
            :getter="system['splash.length']"
            :setter="systemStore.post"
            apiKey="splash.length"
            v-if="system['splash.length']"
            :min="splashLengthOptions.lowerValue"
            :max="splashLengthOptions.higherValue"
            icon="mdi-timer-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.splashScreen.splashLength.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.system.splashScreen.splashLength.help.0')"></li>
                <li v-html="$t('settings.system.splashScreen.splashLength.help.1')"></li>
                <li v-html="$t('settings.system.splashScreen.splashLength.help.2')"></li>
              </ul>
            </template>
          </WrappedSlider>
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.specialKey.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.system.specialKey.behaviourSelect.label"
            :options="specialkeysOptions"
            :getter="system['emulators.specialkeys']"
            :setter="systemStore.post"
            apiKey="emulators.specialkeys"
            v-if="system['emulators.specialkeys']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.specialKey.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.system.specialKey.help.default')"></li>
                <li v-html="$t('settings.system.specialKey.help.nomenu')"></li>
                <li v-html="$t('settings.system.specialKey.help.none')"></li>
              </ul>
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.updates.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.system.updates.toggleButtonLabel"
            :getter="updates.enabled"
            :setter="updateStore.post"
            apiKey="enabled"
            v-if="updates.enabled"
          />
          <WrappedSelect
            label="settings.system.updates.versionType.label"
            :options="updatesTypeOptions"
            :getter="updates.type"
            :setter="updateStore.post"
            apiKey="type"
            v-if="updates.type"
          />
        </template>
      </FormFragmentContainer>

    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6 col">
      <FormFragmentContainer title="settings.system.services.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.system.services.managerEnabled"
            :getter="system['manager.enabled']"
            :setter="systemStore.post"
            apiKey="manager.enabled"
            v-if="system['manager.enabled']"
          />
          <WrappedToggle
            label="settings.system.services.sambaEnabled"
            :getter="system['samba.enabled']"
            :setter="systemStore.post"
            apiKey="samba.enabled"
            v-if="system['samba.enabled']"
          />
          <WrappedToggle
            label="settings.system.services.virtualGamepadsEnabled"
            :getter="system['virtual-gamepads.enabled']"
            :setter="systemStore.post"
            apiKey="virtual-gamepads.enabled"
            v-if="system['virtual-gamepads.enabled']"
          />
          <WrappedToggle
            label="settings.system.services.sshEnabled"
            :getter="system['ssh.enabled']"
            :setter="systemStore.post"
            apiKey="ssh.enabled"
            v-if="system['ssh.enabled']"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.system.demo.title">
        <template v-slot:content>
          <WrappedMultipleSelect
            label="settings.system.demo.systemlist.label"
            :options="demoSystemlistOptions"
            :getter="global['demo.systemlist']"
            :setter="globalStore.post"
            apiKey="demo.systemlist"
            v-if="global['demo.systemlist']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.demo.systemlist.help') }}
            </template>
          </WrappedMultipleSelect>
          <WrappedSlider
            label="settings.system.demo.duration.label"
            :getter="global['demo.duration']"
            :setter="globalStore.post"
            apiKey="demo.duration"
            v-if="global['demo.duration']"
            :min="demoDurationOptions.lowerValue"
            :max="demoDurationOptions.higherValue"
            icon="mdi-timer-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.demo.duration.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.system.demo.infoscreenduration.label"
            :getter="global['demo.infoscreenduration']"
            :setter="globalStore.post"
            apiKey="demo.infoscreenduration"
            v-if="global['demo.infoscreenduration']"
            :min="demoInfoscreendurationOptions.lowerValue"
            :max="demoInfoscreendurationOptions.higherValue"
            icon="mdi-timer-outline"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.demo.infoscreenduration.help') }}
            </template>
          </WrappedSlider>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import { useSystemStore } from 'stores/configuration/system';
import { useUpdatesStore } from 'stores/configuration/updates';
import { storeToRefs } from 'pinia';
import WrappedMultipleSelect from 'components/ui-kit/WrappedMultipleSelect.vue';
import { useGlobalStore } from 'stores/configuration/global';
import WrappedSlider from 'components/ui-kit/WrappedSlider.vue';

const updateStore = useUpdatesStore();
updateStore.fetch();
const { updatesTypeOptions, updates } = storeToRefs(updateStore);

const systemStore = useSystemStore();
const {
  languageOptions,
  kblayoutOptions,
  timezoneOptions,
  specialkeysOptions,
  splashLengthOptions,
  system,
} = storeToRefs(systemStore);

const globalStore = useGlobalStore();
globalStore.fetch();
const {
  demoInfoscreendurationOptions,
  demoDurationOptions,
  demoSystemlistOptions,
  global,
} = storeToRefs(globalStore);
</script>

<style lang="sass" scoped>
.settings
  .q-tab-panels
    .line
      margin-bottom: 1em
      background: white

  @media(max-width: 700px)
    .settings
      .q-tab-panels
        .line
          .title
            text-align: right
</style>
