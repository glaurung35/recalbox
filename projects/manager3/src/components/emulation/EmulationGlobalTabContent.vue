<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12 col-lg-12 col-xl-6">
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.ratio.title">
          <template v-slot:content>
            <WrappedSelect
              label="emulation.systems.ratio.label"
              :options="ratioOptions"
              getter="global/ratio"
              setter="global/post"
              apiKey="ratio"
            />
          </template>
        </FormFragmentContainer>
      </div>
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.rewind.title">
          <template v-slot:content>
            <WrappedToggle
              label="emulation.systems.rewind.label"
              getter="global/rewind"
              setter="global/post"
              apiKey="rewind"
            />
            <p class="help">
              <q-icon class="text-red" name="mdi-alert" style="font-size: 2em"/>
              {{$t('emulation.systems.rewind.warning')}}
            </p>
          </template>
        </FormFragmentContainer>
      </div>
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.autosave.title">
          <template v-slot:content>
            <WrappedToggle
              label="emulation.systems.autosave.label"
              getter="global/autosave"
              setter="global/post"
              apiKey="autosave"
            />
            <p class="help" v-html="$t('emulation.systems.autosave.paragraph')"></p>
          </template>
        </FormFragmentContainer>
      </div>
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.shaders.title">
          <template v-slot:content>
            <p class="help" v-html="$t('emulation.systems.shaders.paragraph')"></p>
            <WrappedSelect
              label="emulation.systems.shaders.label"
              :options="shadersetOptions"
              getter="global/shaderset"
              setter="global/post"
              apiKey="shaderset"
            />
            <WrappedSelect
              label="emulation.systems.customshaders.label"
              :options="shadersetfileOptions"
              getter="global/shadersetfile"
              setter="global/post"
              apiKey="shaderset.file"
              :disable="shaderset.value !== 'custom'"
            />
          </template>
        </FormFragmentContainer>
      </div>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-12 col-lg-12 col-xl-6">
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.integerscale.title">
          <template v-slot:content>
            <WrappedToggle
              label="emulation.systems.integerscale.label"
              getter="global/integerscale"
              setter="global/post"
              apiKey="integerscale"
            />
          </template>
        </FormFragmentContainer>
      </div>
      <div class="col col-xs-12">
        <FormFragmentContainer title="emulation.systems.retroachievements.title">
          <template v-slot:content>
            <p v-html="$t('emulation.systems.retroachievements.paragraph_1')"></p>
            <p v-html="$t('emulation.systems.retroachievements.paragraph_2')"></p>

            <WrappedToggle
              label="emulation.systems.retroachievements.activate"
              getter="global/retroachievements"
              setter="global/post"
              apiKey="retroachievements"
            />

            <WrappedTextInput
              label="emulation.systems.retroachievements.username"
              getter="global/username"
              setter="global/post"
              apiKey="retroachievements.username"
            />
            <WrappedTextInput
              label="emulation.systems.retroachievements.password"
              getter="global/password"
              setter="global/post"
              apiKey="retroachievements.password"
              password
            />

            <p class="help">
              <q-icon class="text-red" name="mdi-alert" style="font-size: 2em"/>
              {{$t('emulation.systems.retroachievements.paragraph_3')}}
            </p>

            <WrappedToggle
              label="emulation.systems.retroachievements.hardcore"
              getter="global/hardcore"
              setter="global/post"
              apiKey="retroachievements.hardcore"
            />
          </template>
        </FormFragmentContainer>
      </div>
    </div>
  </div>
</template>

<script>
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedTextInput from 'components/global/WrappedTextInput'
  import WrappedToggle from 'components/global/WrappedToggle'
  import FormFragmentContainer from '../global/FormFragmentContainer'

  export default {
    name: "EmulationGlobalTabContent",
    components: {
      FormFragmentContainer,
      WrappedSelect,
      WrappedTextInput,
      WrappedToggle,
    },
    created() {
      this.$store.dispatch('global/get')
    },
    computed: {
      systems: {
        get: function () {
          return this.$store.getters['systems/list'] ? this.$store.getters['systems/list'] : []
        },
      },
      ratioOptions: {
        get: function () {
          return this.$store.getters['global/ratioOptions'] ? this.$store.getters['global/ratioOptions'] : []
        }
      },
      rewind: {
        get: function () {
          return this.$store.getters['global/rewind'] ? this.$store.getters['global/rewind'] : 0
        }
      },
      autosave: {
        get: function () {
          return this.$store.getters['global/autosave'] ? this.$store.getters['global/autosave'] : 0
        }
      },
      shaderset: {
        get: function () {
          return this.$store.getters['global/shaderset'] ? this.$store.getters['global/shaderset'] : []
        }
      },
      shadersetOptions: {
        get: function () {
          return this.$store.getters['global/shadersetOptions'] ? this.$store.getters['global/shadersetOptions'] : []
        }
      },
      shadersetfileOptions: {
        get: function () {
          return this.$store.getters['global/shadersetfileOptions'] ? this.$store.getters['global/shadersetfileOptions'] : []
        }
      },
      integerscale: {
        get: function () {
          return this.$store.getters['global/integerscale'] ? this.$store.getters['global/integerscale'] : 0
        }
      },
      retroachievements: {
        get: function () {
          return this.$store.getters['global/retroachievements'] ? this.$store.getters['global/retroachievements'] : 0
        }
      },
      username: {
        get: function () {
          return this.$store.getters['global/username'] ? this.$store.getters['global/username'] : ''
        }
      },
      password: {
        get: function () {
          return this.$store.getters['global/password'] ? this.$store.getters['global/password'] : ''
        }
      },
      hardcore: {
        get: function () {
          return this.$store.getters['global/hardcore'] ? this.$store.getters['global/hardcore'] : 0
        }
      }
    },
    data() {
      return {
        modelRatio: null,
        modelShaders: null,
        valueAntialiasing: false,
        valueRewind: false,
        valueRetroachievements: false,
        loginRetroachievements: '',
        passwordRetroachievements: '',
        hardcoreRetroachievements: false,
        isPwd: true,
        menuShadersOptions: [
          {
            label: '--',
            value: 'null',
          },
          {
            label: 'Retro',
            value: 'retro',
          },
          {
            label: 'Scanlines',
            value: 'scanlines',
          },
        ],
        startFromFirstSystem: true,
        disableSystemSelection: true,
        showOnlyScrapedGames: true,
      }
    },
  }
</script>

<style lang="sass" scoped>

</style>
