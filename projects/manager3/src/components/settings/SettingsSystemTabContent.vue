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
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.internationalization.langSelect.help') }}
            </template>
          </WrappedSelect>
          <WrappedSelect
            label="settings.system.internationalization.keyboardSelect.label"
            :options="kblayoutOptions"
            :getter="system.kblayout"
            :setter="systemStore.post"
            apiKey="kblayout"
            v-if="system.kblayout"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.internationalization.keyboardSelect.help') }}
            </template>
          </WrappedSelect>
          <WrappedSelect
            label="settings.system.internationalization.timeZoneSelect.label"
            :options="timezoneOptions"
            :getter="system.timezone"
            :setter="systemStore.post"
            apiKey="timezone"
            v-if="system.timezone"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.internationalization.timeZoneSelect.help') }}
            </template>
          </WrappedSelect>
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
          <WrappedSelect
            label="settings.system.splashScreen.splashSelect.label"
            :options="splashSelectOptions"
            :getter="system['splash.select']"
            :setter="systemStore.post"
            apiKey="splash.select"
            v-if="system['splash.select']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.splashScreen.splashSelect.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.system.splashScreen.splashSelect.help.0')"></li>
                <li v-html="$t('settings.system.splashScreen.splashSelect.help.1')"></li>
                <li v-html="$t('settings.system.splashScreen.splashSelect.help.2')"></li>
              </ul>
            </template>
          </WrappedSelect>
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
            label="settings.system.updates.enabled.label"
            :getter="updates.enabled"
            :setter="updateStore.post"
            apiKey="enabled"
            v-if="updates.enabled"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.updates.enabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.hat.wpaf.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.system.hat.wpaf.enabled.label"
            :getter="hat['wpaf.enabled']"
            :setter="hatStore.post"
            apiKey="wpaf.enabled"
            v-if="hat['wpaf.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.hat.wpaf.enabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedSelect
            label="settings.system.hat.wpaf.board.label"
            :options="wpafBoardOptions"
            :getter="hat['wpaf.board']"
            :setter="hatStore.post"
            apiKey="wpaf.board"
            v-if="hat['wpaf.board']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.hat.wpaf.board.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.system.hat.wpaf.board.help.0')"></li>
                <li v-html="$t('settings.system.hat.wpaf.board.help.1')"></li>
                <li v-html="$t('settings.system.hat.wpaf.board.help.2')"></li>
                <li v-html="$t('settings.system.hat.wpaf.board.help.3')"></li>
                <li v-html="$t('settings.system.hat.wpaf.board.help.4')"></li>
              </ul>
            </template>
          </WrappedSelect>
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.brightness.title">
        <template v-slot:content>
          <WrappedSlider
            label="settings.system.brightness.label"
            :options="brightnessOptions"
            :getter="emulationstation.brightness"
            :setter="emulationstationStore.post"
            apiKey="brightness"
            v-if="emulationstation.brightness"
            :mix="brightnessOptions.lowerValue"
            :max="brightnessOptions.higherValue"
            icon="mdi-brightness-6"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.brightness.help') }}
            </template>
          </WrappedSlider>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6 col">
      <FormFragmentContainer title="settings.system.services.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.system.services.managerEnabled.label"
            :getter="system['manager.enabled']"
            :setter="systemStore.post"
            apiKey="manager.enabled"
            v-if="system['manager.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.services.managerEnabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.system.services.sambaEnabled.label"
            :getter="system['samba.enabled']"
            :setter="systemStore.post"
            apiKey="samba.enabled"
            v-if="system['samba.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.services.sambaEnabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.system.services.virtualGamepadsEnabled.label"
            :getter="system['virtual-gamepads.enabled']"
            :setter="systemStore.post"
            apiKey="virtual-gamepads.enabled"
            v-if="system['virtual-gamepads.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.services.virtualGamepadsEnabled.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.system.services.sshEnabled.label"
            :getter="system['ssh.enabled']"
            :setter="systemStore.post"
            apiKey="ssh.enabled"
            v-if="system['ssh.enabled']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.services.sshEnabled.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.system.screensaver.title">
        <template v-slot:content>
          <WrappedSlider
            label="settings.system.screensaver.time.label"
            :getter="emulationstation['screensaver.time']"
            :setter="emulationstationStore.post"
            apiKey="screensaver.time"
            v-if="emulationstation['screensaver.time']"
            :mix="screensaverTimeOptions.lowerValue"
            :max="screensaverTimeOptions.higherValue"
            icon="mdi-clock-start"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.screensaver.time.help') }}
            </template>
          </WrappedSlider>
          <WrappedSelect
            label="settings.system.screensaver.type.label"
            :options="screensaverTypeOptions"
            :getter="emulationstation['screensaver.type']"
            :setter="emulationstationStore.post"
            apiKey="screensaver.type"
            v-if="emulationstation['screensaver.type']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.screensaver.type.help.availableOptions') }}
              <ul>
                <li v-html="$t('settings.system.screensaver.type.help.dim')"></li>
                <li v-html="$t('settings.system.screensaver.type.help.black')"></li>
                <li v-html="$t('settings.system.screensaver.type.help.demo')"></li>
                <li v-html="$t('settings.system.screensaver.type.help.gameclip')"></li>
              </ul>
            </template>
          </WrappedSelect>
          <WrappedMultipleSelect
            label="settings.system.screensaver.demo.systemlist.label"
            :options="demoSystemlistOptions"
            :getter="global['demo.systemlist']"
            :setter="globalStore.post"
            apiKey="demo.systemlist"
            v-if="global['demo.systemlist']"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.screensaver.demo.systemlist.help') }}
            </template>
          </WrappedMultipleSelect>
          <WrappedSlider
            label="settings.system.screensaver.demo.duration.label"
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
              {{ $t('settings.system.screensaver.demo.duration.help') }}
            </template>
          </WrappedSlider>
          <WrappedSlider
            label="settings.system.screensaver.demo.infoscreenduration.label"
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
              {{ $t('settings.system.screensaver.demo.infoscreenduration.help') }}
            </template>
          </WrappedSlider>
          <WrappedToggle
            label="settings.system.screensaver.gameclip.showhelpitems.label"
            :getter="emulationstation.showgamecliphelpitems"
            :setter="emulationstationStore.post"
            apiKey="showgamecliphelpitems"
            v-if="emulationstation.showgamecliphelpitems"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.screensaver.gameclip.showhelpitems.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.system.screensaver.gameclip.showclippingitem.label"
            :getter="emulationstation.showgameclipclippingitem"
            :setter="emulationstationStore.post"
            apiKey="showgameclipclippingitem"
            v-if="emulationstation.showgameclipclippingitem"
            help
          >
            <template v-slot:help>
              {{ $t('settings.system.screensaver.gameclip.showclippingitem.help') }}
            </template>
          </WrappedToggle>
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
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useHatStore } from 'stores/configuration/hat';
import { storeToRefs } from 'pinia';
import WrappedMultipleSelect from 'components/ui-kit/WrappedMultipleSelect.vue';
import { useGlobalStore } from 'stores/configuration/global';
import WrappedSlider from 'components/ui-kit/WrappedSlider.vue';

const updateStore = useUpdatesStore();
updateStore.fetch();
const { updates } = storeToRefs(updateStore);

const systemStore = useSystemStore();
systemStore.fetch();
const {
  languageOptions,
  kblayoutOptions,
  timezoneOptions,
  specialkeysOptions,
  splashLengthOptions,
  splashSelectOptions,
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

const emulationstationStore = useEmulationstationStore();
emulationstationStore.fetch();
const {
  screensaverTimeOptions,
  screensaverTypeOptions,
  brightnessOptions,
  emulationstation,
} = storeToRefs(emulationstationStore);

const hatStore = useHatStore();
hatStore.fetch();
const {
  wpafBoardOptions,
  hat,
} = storeToRefs(hatStore);
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
