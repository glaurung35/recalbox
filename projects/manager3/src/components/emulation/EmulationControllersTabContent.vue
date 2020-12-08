<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="emulation.controllers.db9.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.db9.activate"
            getter="controllers/db9"
            setter="controllers/post"
            apiKey="db9.enabled"
          />

          <WrappedTextInput
            label="emulation.controllers.db9.settings"
            getter="controllers/db9args"
            setter="controllers/post"
            apiKey="db9.args"
          />

        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="emulation.controllers.gamecon.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gamecon.activate"
            getter="controllers/gamecon"
            setter="controllers/post"
            apiKey="gamecon.enabled"
          />

          <WrappedTextInput
            label="emulation.controllers.gamecon.settings"
            getter="controllers/gameconargs"
            setter="controllers/post"
            apiKey="gamecon.args"
          />

        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="emulation.controllers.gpio.title">
        <template v-slot:content>
          <WrappedToggle
            label="emulation.controllers.gpio.activate"
            getter="controllers/gpio"
            setter="controllers/post"
            apiKey="gpio.enabled"
          />

          <WrappedTextInput
            label="emulation.controllers.gpio.settings"
            getter="controllers/gpioargs"
            setter="controllers/post"
            apiKey="gpio.args"
          />

        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="emulation.controllers.ps3.title">
        <template v-slot:content>
          <q-toggle
            :label="$t('emulation.controllers.ps3.activate')"
            class="q-mb-md"
            v-model="valuePs3"
          />
          <WrappedSelect
            label="emulation.controllers.ps3.label"
            :options="ps3driverOptions"
            getter="controllers/ps3driver"
            setter="controllers/post"
            apiKey="ps3.driver"
          />

        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script>
  import FormFragmentContainer from "components/global/FormFragmentContainer"
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedTextInput from 'components/global/WrappedTextInput'
  import WrappedToggle from 'components/global/WrappedToggle'

  export default {
    name: 'EmulationControllersTabContent',
    components: {
      FormFragmentContainer,
      WrappedSelect,
      WrappedToggle,
      WrappedTextInput,
    },
    created() {
      this.$store.dispatch('controllers/get')
    },
    computed: {
      db9: {
        get: function () {
          return this.$store.getters['controllers/db9'] ? this.$store.getters['controllers/db9'] : 0
        }
      },
      db9args: {
        get: function () {
          return this.$store.getters['controllers/db9args'] ? this.$store.getters['controllers/db9args'] : ''
        }
      },
      gamecon: {
        get: function () {
          return this.$store.getters['controllers/gamecon'] ? this.$store.getters['controllers/gamecon'] : 0
        }
      },
      gameconargs: {
        get: function () {
          return this.$store.getters['controllers/gameconargs'] ? this.$store.getters['controllers/gameconargs'] : ''
        }
      },
      gpio: {
        get: function () {
          return this.$store.getters['controllers/gpio'] ? this.$store.getters['controllers/gpio'] : 0
        }
      },
      gpioargs: {
        get: function () {
          return this.$store.getters['controllers/gpioargs'] ? this.$store.getters['controllers/gpioargs'] : ''
        }
      },
      ps3: {
        get: function () {
          return this.$store.getters['controllers/ps3'] ? this.$store.getters['controllers/ps3'] : 0
        }
      },
      ps3driver: {
        get: function () {
          return this.$store.getters['controllers/ps3driver'] ? this.$store.getters['controllers/ps3driver'] : ''
        }
      },
      ps3driverOptions: {
        get: function () {
          return this.$store.getters['controllers/ps3driverOptions'] ? this.$store.getters['controllers/ps3driverOptions'] : []
        }
      },
    },
    data() {
      return {
        valuePs3: false,
        ps3Driver: '',
        menuPs3Options: [
          {
            label: '--',
            value: 'none',
          },
          {
            label: 'BlueZ 5',
            value: 'bluez5',
          },
          {
            label: 'Official',
            value: 'official',
          },
          {
            label: 'Shanwan',
            value: 'shanwan',
          },
        ],
      }
    },
  }
</script>

<style lang="sass" scoped>

</style>
