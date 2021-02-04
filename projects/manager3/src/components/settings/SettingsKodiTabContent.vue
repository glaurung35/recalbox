<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.status.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.enableKodi"
              getter="kodi/enabled"
              setter="kodi/post"
              apiKey="enabled"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.kodiAtStartup"
              getter="kodi/atStartup"
              setter="kodi/post"
              apiKey="atstartup"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.status.launchWithX"
              getter="kodi/xButton"
              setter="kodi/post"
              apiKey="xbutton"
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
              getter="kodi/videoMode"
              setter="kodi/post"
              apiKey="videomode"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.kodi.network.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <p class="help" v-html="$t('settings.kodi.network.waitMode.paragraph_1')"></p>
            <WrappedSelect
              label="settings.kodi.network.waitMode.select.options.label"
              :options="waitModeOptions"
              getter="kodi/waitMode"
              setter="kodi/post"
              apiKey="network.waitmode"
            />
            <WrappedTextInput
              label="settings.kodi.network.waitHost"
              getter="kodi/waitHost"
              setter="kodi/post"
              apiKey="network.waithost"
            />
            <p class="help">{{ $t('settings.kodi.network.waitTime.paragraph_1') }}</p>
            <WrappedTextInput
              label="settings.kodi.network.waitTime.title"
              getter="kodi/waitTime"
              setter="kodi/post"
              apiKey="network.waittime"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script>
  import FormFragmentContainer from 'components/global/FormFragmentContainer'
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedTextInput from 'components/global/WrappedTextInput'
  import WrappedToggle from 'components/global/WrappedToggle'

  export default {
    name: 'SettingsKodiTabContent',
    components: {
      FormFragmentContainer,
      WrappedTextInput,
      WrappedToggle,
      WrappedSelect
    },
    created() {
      this.$store.dispatch('kodi/get')
    },
    computed: {
      videoModeOptions: {
        get: function() {
          return this.$store.getters['kodi/videoModeOptions'] ? this.$store.getters['kodi/videoModeOptions'] : []
        }
      },
      waitModeOptions: {
        get: function() {
          return this.$store.getters['kodi/waitModeOptions'] ? this.$store.getters['kodi/waitModeOptions'] : []
        }
      },
    },
    data() {
      return {
        enableKodi: true,
        kodiAtStartup: true,
        launchWithX: true,
      }
    }
  }
</script>

<style lang="sass" scoped>

</style>
