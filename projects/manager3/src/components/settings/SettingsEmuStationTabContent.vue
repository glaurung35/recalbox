<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.emustation.options.title">
        <template v-slot:content>
          <p class="help" v-html="$t('settings.emustation.options.paragraph_1')"></p>
          <WrappedSelect
            label="settings.emustation.options.menuStyle.label"
            :options="menuOptions"
            getter="emulationstation/menu"
            setter="emulationstation/post"
            apiKey="menu"
          />
          <WrappedSelect
            label="settings.emustation.options.systemSelectedAsDefault.label"
            :options="selectedsystemOptions"
            getter="emulationstation/selectedsystem"
            setter="emulationstation/post"
            apiKey="selectedsystem"
          />
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <div class="blank-container">
        <p class="help" v-html="$t('settings.emustation.options.paragraph_2')"></p>
        <WrappedToggle
          label="settings.emustation.options.startFromFirstSystem"
          getter="emulationstation/bootongamelist"
          setter="emulationstation/post"
          apiKey="bootongamelist"
        />

        <p class="help" v-html="$t('settings.emustation.options.paragraph_3')"></p>
        <WrappedToggle
          label="settings.emustation.options.disableSystemSelection"
          getter="emulationstation/hidesystemview"
          setter="emulationstation/post"
          apiKey="hidesystemview"
        />

        <p class="help" v-html="$t('settings.emustation.options.paragraph_4')"></p>
        <WrappedToggle
          label="settings.emustation.options.showOnlyScrapedGames"
          getter="emulationstation/gamelistonly"
          setter="emulationstation/post"
          apiKey="gamelistonly"
        />
      </div>
    </div>
  </div>
</template>

<script>
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedToggle from 'components/global/WrappedToggle'
  import FormFragmentContainer from '../global/FormFragmentContainer'

  export default {
    name: 'SettingsEmuStationTabContent',
    components: {
      WrappedSelect,
      WrappedToggle,
      FormFragmentContainer,
    },
    created() {
      this.$store.dispatch('emulationstation/get')
    },
    computed: {
      menuOptions: {
        get: function() {
          return this.$store.getters['emulationstation/menuOptions'] ? this.$store.getters['emulationstation/menuOptions'] : []
        }
      },
      selectedsystemOptions: {
        get: function() {
          return this.$store.getters['emulationstation/selectedsystemOptions'] ? this.$store.getters['emulationstation/selectedsystemOptions'] : []
        }
      }
    }
  }
</script>

<style lang="sass" scoped>

</style>
