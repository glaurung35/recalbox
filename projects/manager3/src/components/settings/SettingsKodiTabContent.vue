<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.status.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.enableKodi"
              :getter="kodi.enabled"
              :setter="kodiStore.post"
              apiKey="enabled"
              v-if="kodi.enabled"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.kodiAtStartup"
              :getter="kodi.atstartup"
              :setter="kodiStore.post"
              apiKey="atstartup"
              v-if="kodi.atstartup"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.launchWithX"
              :getter="kodi.xbutton"
              :setter="kodiStore.post"
              apiKey="xbutton"
              v-if="kodi.xbutton"
            />
          </div>
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
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.network.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.kodi.network.waitMode.select.options.label"
              :options="waitModeOptions"
              :getter="kodi['network.waitmode']"
              :setter="kodiStore.post"
              apiKey="network.waitmode"
              v-if="kodi['network.waitmode']"
            />
            <p class="help" v-html="$t('settings.kodi.network.waitMode.help')"></p>

          <q-separator/>

            <WrappedTextInput
              label="settings.kodi.network.waitHost"
              :getter="kodi['network.waithost']"
              :setter="kodiStore.post"
              apiKey="network.waithost"
              v-if="kodi['network.waithost']"
            />

          <q-separator/>

            <WrappedTextInput
              label="settings.kodi.network.waitTime.title"
              :getter="kodi['network.waittime']"
              :setter="kodiStore.post"
              apiKey="network.waittime"
              v-if="kodi['network.waittime']"
            />
            <p class="help">{{ $t('settings.kodi.network.waitTime.help') }}</p>
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { useKodiStore } from 'stores/kodi';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';

const kodiStore = useKodiStore();
kodiStore.fetch();
const { videoModeOptions, waitModeOptions, kodi } = storeToRefs(kodiStore);
</script>
