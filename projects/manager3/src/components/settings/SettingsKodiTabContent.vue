<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.kodi.options.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.options.enableKodi"
              getter="kodi/enabled"
              setter="kodi/post"
              apiKey="enabled"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.options.kodiAtStartup"
              getter="kodi/atStartup"
              setter="kodi/post"
              apiKey="atstartup"
            />
          </div>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.kodi.options.launchWithX"
              getter="kodi/xButton"
              setter="kodi/post"
              apiKey="xbutton"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script>
  import WrappedToggle from 'components/global/WrappedToggle'
  import FormFragmentContainer from '../global/FormFragmentContainer'

  export default {
    name: 'SettingsKodiTabContent',
    components: {
      WrappedToggle,
      FormFragmentContainer,
    },
    created() {
      this.$store.dispatch('kodi/get')
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
