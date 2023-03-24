<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-7">
      <FormFragmentContainer title="settings.system.settings.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.system.settings.langSelect.label"
            :options="languageOptions"
            :getter="system.language"
            :setter="systemStore.post"
            apiKey="language"
            v-if="system.language"
          />
          <WrappedSelect
            label="settings.system.settings.keyboardSelect.label"
            :options="kblayoutOptions"
            :getter="system.kblayout"
            :setter="systemStore.post"
            apiKey="kblayout"
            v-if="system.kblayout"
          />
          <WrappedSelect
            label="settings.system.settings.timeZoneSelect.label"
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
    <div class="col col-xs-12 col-sm-12 col-md-5 col">
      <div class="row items-stretch line">
        <div class="col-12 title vertical-middle self-center text-center">
          {{ $t('settings.system.versions.title') }}
        </div>
        <div class="col-12" style="padding: 0 1em;">
          <div class="row">
            <div class="col col-xs-12 col-sm-12 col-md-6">
              <div
                @click="copyToClipboard('Linux : ' + versions.linux)"
                class="row system-versions"
              >
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  Linux
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip
                    color="light-blue"
                    dense
                    icon="mdi-source-branch"
                    square
                    text-color="white"
                  >
                    {{versions.linux}}
                  </q-chip>
                </div>
              </div>
              <div
                @click="copyToClipboard('Recalbox : ' + versions.recalbox)"
                class="row system-versions"
              >
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  Recalbox
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip
                    color="light-blue"
                    dense
                    icon="mdi-source-branch"
                    square
                    text-color="white"
                  >
                    {{versions.recalbox}}
                  </q-chip>
                </div>
              </div>
            </div>
            <div
              @click="copyToClipboard('WebAPI : ' + versions.webapi)"
              class="col col-xs-12 col-sm-12 col-md-6"
            >
              <div class="row system-versions">
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  WebAPI
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip
                    color="light-blue"
                    dense
                    icon="mdi-source-branch"
                    square
                    text-color="white"
                  >
                    {{versions.webapi}}
                  </q-chip>
                </div>
              </div>
              <div
                @click="copyToClipboard('RetroArch : ' + versions.libretro.retroarch)"
                class="row system-versions"
              >
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  RetroArch
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip
                    color="light-blue"
                    dense
                    icon="mdi-source-branch"
                    square
                    text-color="white"
                  >
                    {{versions.libretro.retroarch}}
                  </q-chip>
                </div>
              </div>
            </div>
          </div>
          <q-separator style="margin-top: 1em; margin-bottom: 1em"/>
        </div>

        <LongDataList
          :data="versions.libretro.cores"
          height="calc(660px + 2em - 87px - 50.38px)"
          v-if="versions.libretro.cores"
        />

      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import LongDataList from 'components/global/LongDataList.vue';
import { copyToClipboard } from 'src/tools';
import { useSystemStore } from 'stores/system';
import { useUpdatesStore } from 'stores/updates';
import { useVersionsStore } from 'stores/versions';
import { storeToRefs } from 'pinia';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';

const updateStore = useUpdatesStore();
updateStore.fetch();
const { updatesTypeOptions, updates } = storeToRefs(updateStore);

const versionsStore = useVersionsStore();
const { versions } = storeToRefs(versionsStore);

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

      .system-versions,
      .version-line
        &:hover
          background: $rc-input-grey
          cursor: pointer

          .q-chip
            background: $primary !important
            color: $accent !important

  @media(max-width: 700px)
    .settings
      .q-tab-panels
        .line
          .title
            text-align: right
</style>
