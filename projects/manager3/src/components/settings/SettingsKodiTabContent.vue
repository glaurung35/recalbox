<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.status.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.kodi.status.enableKodi.label"
            :getter="kodi.enabled"
            :setter="kodiStore.post"
            apiKey="enabled"
            v-if="kodi.enabled"
            help
          >
            <template v-slot:help>
              {{ $t('settings.kodi.status.enableKodi.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.kodi.status.kodiAtStartup.label"
            :getter="kodi.atstartup"
            :setter="kodiStore.post"
            apiKey="atstartup"
            v-if="kodi.atstartup"
            help
          >
            <template v-slot:help>
              {{ $t('settings.kodi.status.kodiAtStartup.help') }}
            </template>
          </WrappedToggle>
          <WrappedToggle
            label="settings.kodi.status.launchWithX.label"
            :getter="kodi.xbutton"
            :setter="kodiStore.post"
            apiKey="xbutton"
            v-if="kodi.xbutton"
            help
          >
            <template v-slot:help>
              {{ $t('settings.kodi.status.launchWithX.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.kodi.video.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.kodi.video.select.options.label"
              :options="videoModeOptions"
              :getter="kodi.videomode"
              :setter="kodiStore.post"
              apiKey="videomode"
              v-if="kodi.videomode"
              help
            >
              <template v-slot:help>
                {{ $t('settings.kodi.video.select.options.help') }}
              </template>
            </WrappedSelect>
          </div>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.network.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.kodi.network.waitMode.label"
              :options="waitModeOptions"
              :getter="kodi['network.waitmode']"
              :setter="kodiStore.post"
              apiKey="network.waitmode"
              v-if="kodi['network.waitmode']"
              clearable
              help
            >
              <template v-slot:help>
                {{ $t('settings.kodi.network.waitMode.help.availableOptions') }}
                <ul>
                  <li v-html="$t('settings.kodi.network.waitMode.help.required')"></li>
                  <li v-html="$t('settings.kodi.network.waitMode.help.wish')"></li>
                  <li v-html="$t('settings.kodi.network.waitMode.help.nonce')"></li>
                </ul>
              </template>
            </WrappedSelect>

            <q-separator/>

            <WrappedTextInput
              label="settings.kodi.network.waitHost.label"
              :getter="kodi['network.waithost']"
              :setter="kodiStore.post"
              apiKey="network.waithost"
              v-if="kodi['network.waithost']"
              clearable
              help
            >
            <template v-slot:help>
              {{ $t('settings.kodi.network.waitHost.help') }}
            </template>
            </WrappedTextInput>

            <q-separator/>

            <WrappedTextInput
              label="settings.kodi.network.waitTime.label"
              :getter="kodi['network.waittime']"
              :setter="kodiStore.post"
              apiKey="network.waittime"
              v-if="kodi['network.waittime']"
              help
            >
              <template v-slot:help>
                {{ $t('settings.kodi.network.waitTime.help') }}
              </template>
            </WrappedTextInput>
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { useKodiStore } from 'stores/configuration/kodi';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedTextInput from 'components/ui-kit/WrappedTextInput.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';

const kodiStore = useKodiStore();
kodiStore.fetch();
const { videoModeOptions, waitModeOptions, kodi } = storeToRefs(kodiStore);
</script>
