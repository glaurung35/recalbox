<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-7">
      <FormFragmentContainer title="settings.system.settings.title">
        <template v-slot:content>
          <WrappedSelect
            label="settings.system.settings.langSelect.label"
            :options="languageOptions"
            getter="system/language"
            setter="system/post"
            apiKey="language"
          />
          <WrappedSelect
            label="settings.system.settings.keyboardSelect.label"
            :options="kblayoutOptions"
            getter="system/kblayout"
            setter="system/post"
            apiKey="kblayout"
          />
          <WrappedSelect
            label="settings.system.settings.timeZoneSelect.label"
            :options="timezoneOptions"
            getter="system/timezone"
            setter="system/post"
            apiKey="timezone"
          />
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.specialKey.title">
        <template v-slot:content>
          <p>{{ $t('settings.system.specialKey.paragraph_1') }}</p>

          <p class="help" v-html="$t('settings.system.specialKey.paragraph_2')"></p>

          <WrappedSelect
            label="settings.system.specialKey.behaviourSelect.label"
            :options="specialkeysOptions"
            getter="system/specialkeys"
            setter="system/post"
            apiKey="emulators.specialkeys"
          />
        </template>
      </FormFragmentContainer>

      <FormFragmentContainer title="settings.system.updates.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.system.updates.toggleButtonLabel"
            getter="updates/enabled"
            setter="updates/post"
            apiKey="enabled"
          />
          <WrappedSelect
            label="settings.system.updates.versionType.label"
            :options="typeOptions"
            getter="updates/type"
            setter="updates/post"
            apiKey="type"
          />
        </template>
      </FormFragmentContainer>

    </div>
    <div class="col col-xs-12 col-sm-12 col-md-5">
      <div class="row items-stretch line">
        <div class="col-12 title vertical-middle self-center text-center">
          {{ $t('settings.system.versions.title') }}
        </div>
        <div class="col-12" style="padding: 0 1em;">
          <div class="row">
            <div class="col col-xs-12 col-sm-12 col-md-6">
              <div @click="copyToClipboard('Linux : ' + versions.linux)" class="row system-versions">
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  Linux
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip color="light-blue" dense icon="mdi-source-branch" square text-color="white">
                    {{versions.linux}}
                  </q-chip>
                </div>
              </div>
              <div @click="copyToClipboard('Recalbox : ' + versions.recalbox)" class="row system-versions">
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  Recalbox
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip color="light-blue" dense icon="mdi-source-branch" square text-color="white">
                    {{versions.recalbox}}
                  </q-chip>
                </div>
              </div>
            </div>
            <div @click="copyToClipboard('WebAPI : ' + versions.webapi)" class="col col-xs-12 col-sm-12 col-md-6">
              <div class="row system-versions">
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  WebAPI
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip color="light-blue" dense icon="mdi-source-branch" square text-color="white">
                    {{versions.webapi}}
                  </q-chip>
                </div>
              </div>
              <div @click="copyToClipboard('RetroArch : ' + versions.libretro.retroarch)" class="row system-versions">
                <div class="col col-xs-3 col-md-4 self-center" style="padding-left: .5em;">
                  RetroArch
                </div>
                <div class="col col-xs-9 col-md-8">
                  <q-chip color="light-blue" dense icon="mdi-source-branch" square text-color="white">
                    {{versions.libretro.retroarch}}
                  </q-chip>
                </div>
              </div>
            </div>
          </div>
          <q-separator style="margin-top: 1em; margin-bottom: 1em"/>
        </div>

        <LongDataList
          :data="versions.libretro.cores"
          height="calc(551px + 2em - 87px - 50.38px)"
        />

      </div>
    </div>
  </div>
</template>

<script>
  import WrappedSelect from 'components/global/WrappedSelect'
  import WrappedToggle from 'components/global/WrappedToggle'
  import tools from 'src/tools'
  import FormFragmentContainer from '../global/FormFragmentContainer'
  import LongDataList from '../global/LongDataList'

  export default {
    name: 'SettingsSystemTabContent',
    components: {
      WrappedSelect,
      WrappedToggle,
      FormFragmentContainer,
      LongDataList,
    },
    methods: {
      copyToClipboard: (content) => tools.copyToClipboard(content)
    },
    created() {
      this.$store.dispatch('system/get')
      this.$store.dispatch('updates/get')
      this.$store.dispatch('versions/get')
    },
    computed: {
      languageOptions: {
        get: function() {
          return this.$store.getters['system/languageOptions'] ? this.$store.getters['system/languageOptions'] : []
        }
      },
      kblayoutOptions: {
        get: function() {
          return this.$store.getters['system/kblayoutOptions'] ? this.$store.getters['system/kblayoutOptions'] : []
        }
      },
      timezoneOptions: {
        get: function() {
          return this.$store.getters['system/timezoneOptions'] ? this.$store.getters['system/timezoneOptions'] : []
        }
      },
      specialkeysOptions: {
        get: function() {
          return this.$store.getters['system/specialkeysOptions'] ? this.$store.getters['system/specialkeysOptions'] : []
        }
      },
      typeOptions: {
        get: function() {
          return this.$store.getters['updates/typeOptions'] ? this.$store.getters['updates/typeOptions'] : []
        }
      },
      versions: {
        get: function() {
          return this.$store.getters['versions/list'] ? this.$store.getters['versions/list'] : {}
        }
      }
    }
  }
</script>

<style lang="sass" scoped>
  .settings
    .q-tab-panels
      .line
        margin-bottom: 1em
        background: white

        .system-versions,
        .version-line
          &:hover
            background: $rc-input-grey
            cursor: pointer

            .q-chip
              background: $primary !important
              color: $accent !important

    @media(max-width: 700px)
      .settings
        .q-tab-panels
          .line
            .title
              text-align: right
</style>
