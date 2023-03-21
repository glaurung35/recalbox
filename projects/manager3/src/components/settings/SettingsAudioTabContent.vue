<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.audio.backgroundMusic.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.audio.backgroundMusic.toggleButtonLabel"
            :getter="audio.bgmusic"
            :setter="audioStore.post"
            apiKey="bgmusic"
            v-if="audio.bgmusic"
          />
          <p class="help">{{ $t('settings.audio.backgroundMusic.help') }}</p>
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
          <q-slider
            :max="100"
            :min="0"
            color="accent"
            label
            :model-value="volume"
            @change="value => {volume = value}"
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
import { computed } from 'vue';

const audioStore = useAudioStore();
audioStore.fetch();
const { deviceOptions, audio } = storeToRefs(audioStore);

const volume = computed({
  get: () => audio.value.volume.value,
  set: (value) => audioStore.post({ volume: value }),
});
</script>
