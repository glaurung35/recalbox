<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.audio.backgroundMusic.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.audio.backgroundMusic.toggleButtonLabel"
            :help="$t('settings.audio.backgroundMusic.help')"
            warning
            :getter="audio.bgmusic"
            :setter="audioStore.post"
            apiKey="bgmusic"
            v-if="audio.bgmusic"
          />
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
            clearable
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
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { useAudioStore } from 'stores/audio';
import { storeToRefs } from 'pinia';
import WrappedSlider from 'components/global/WrappedSlider.vue';

const audioStore = useAudioStore();
audioStore.fetch();
const { deviceOptions, volumeOptions, audio } = storeToRefs(audioStore);
</script>
