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
          <WrappedTextInput
            label="settings.system.splashScreen.splashLength.label"
            :getter="system['splash.length']"
            :setter="systemStore.post"
            apiKey="splash.length"
            v-if="system['splash.length']"
          />
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.specialKey.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.system.specialKey.behaviourSelect.label"
            :help="$t('settings.system.specialKey.help')"
            :options="specialkeysOptions"
            :getter="system['emulators.specialkeys']"
            :setter="systemStore.post"
            apiKey="emulators.specialkeys"
            v-if="system['emulators.specialkeys']"
          />
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

    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import { useSystemStore } from 'stores/system';
import { useUpdatesStore } from 'stores/updates';
import { storeToRefs } from 'pinia';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';

const updateStore = useUpdatesStore();
updateStore.fetch();
const { updatesTypeOptions, updates } = storeToRefs(updateStore);

const systemStore = useSystemStore();
systemStore.fetch();
const {
  languageOptions,
  kblayoutOptions,
  timezoneOptions,
  specialkeysOptions,
  system,
} = storeToRefs(systemStore);
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
