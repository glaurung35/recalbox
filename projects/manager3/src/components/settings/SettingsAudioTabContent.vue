<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.audio.backgroundMusic.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.audio.backgroundMusic.toggleButtonLabel"
            help
            warning
            :getter="audio.bgmusic"
            :setter="audioStore.post"
            apiKey="bgmusic"
            v-if="audio.bgmusic"
          >
            <template v-slot:help>
              {{ $t('settings.audio.backgroundMusic.help') }}
            </template>
          </WrappedToggle>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.audio.audioOutput.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.audio.audioOutput.select.options.label"
            :options="deviceOptions"
            :getter="audio.device"
            :setter="audioStore.post"
            apiKey="device"
            v-if="audio.device"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.audio.volume.title">
        <template v-slot:content>
          <WrappedSlider
            :getter="audio.volume"
            :setter="audioStore.post"
            apiKey="volume"
            v-if="audio.volume"
            :min="volumeOptions.lowerValue"
            :max="volumeOptions.higherValue"
            icon="mdi-volume-high"
          />
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import FormFragmentContainer from 'components/ui-kit/FormFragmentContainer.vue';
import WrappedSelect from 'components/ui-kit/WrappedSelect.vue';
import WrappedToggle from 'components/ui-kit/WrappedToggle.vue';
import { useAudioStore } from 'stores/configuration/audio';
import { storeToRefs } from 'pinia';
import WrappedSlider from 'components/ui-kit/WrappedSlider.vue';

const audioStore = useAudioStore();
audioStore.fetch();
const { deviceOptions, volumeOptions, audio } = storeToRefs(audioStore);
</script>
