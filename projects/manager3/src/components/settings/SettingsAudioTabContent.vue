<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.audio.backgroundMusic.title">
        <template v-slot:content>
          <p class="help">{{ $t('settings.audio.backgroundMusic.paragraph_1') }}</p>
          <WrappedToggle
            label="settings.audio.backgroundMusic.toggleButtonLabel"
            getter="audio/bgmusic"
            setter="audio/post"
            apiKey="bgmusic"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.audio.audioOutput.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.audio.audioOutput.select.options.label"
            :options="deviceOptions"
            getter="audio/device"
            setter="audio/post"
            apiKey="device"
          />
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.audio.volume.title">
        <template v-slot:content>
          <q-slider
            :max="100"
            :min="0"
            color="positive"
            label
            :value="volume"
            @change="val => { volume = val }"
          />
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script>
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedToggle from 'components/global/WrappedToggle'
  import FormFragmentContainer from '../global/FormFragmentContainer'

  export default {
    name: 'SettingsAudioTabContent',
    components: {
      FormFragmentContainer,
      WrappedToggle,
      WrappedSelect
    },
    created() {
      this.$store.dispatch('audio/get')
    },
    computed: {
      deviceOptions: {
        get: function() {
          return this.$store.getters['audio/deviceOptions'] ? this.$store.getters['audio/deviceOptions'] : []
        }
      },
      volume: {
        get: function() {
          return this.$store.getters['audio/volume'] ? this.$store.getters['audio/volume'].value : 0
        },
        set: function(value) {
          this.$store.dispatch('audio/post', {volume: value})
        }
      }
    }
  }
</script>

<style lang="sass" scoped>

</style>
