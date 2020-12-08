<template>
  <q-item clickable v-if="lang" v-ripple>
    <q-item-section avatar>
      <q-icon name="mdi-translate"/>
    </q-item-section>
    <q-item-section>
      {{ $t(getTitle()) }}
    </q-item-section>
    <q-menu auto-close>
      <q-list>
        <q-item
          :class="isSelected(langOption.value)"
          :key="langOption.value"
          @click="select(langOption.value)"
          clickable
          v-for="langOption in langOptions"
        >
          <q-item-section avatar>
            <q-img
              :alt="$t(langOption.label)"
              :img-style="{ backgroundSize: 'inherit' }"
              :src="langOption.flag"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label v-html="$t(langOption.label)"></q-item-label>
          </q-item-section>
        </q-item>
      </q-list>
    </q-menu>
  </q-item>
</template>

<script>
  import {LocalStorage} from 'quasar'
  import enFlag from '../../assets/flags/en-us.png'
  import frFlag from '../../assets/flags/fr-fr.png'

  export default {
    name: 'LangSelector',
    data() {
      return {
        lang: this.$i18n.locale,
        langOptions: [
          {value: 'en-us', label: 'i18nMenu.english', flag: enFlag},
          {value: 'fr-fr', label: 'i18nMenu.french', flag: frFlag},
        ],
      }
    },
    methods: {
      select(langValue) {
        this.$i18n.locale = langValue
        try {
          const langIso = langValue === 'fr-fr' ? 'fr' : langValue

          LocalStorage.set('locale', langValue)

          import(`quasar/lang/${langIso}`).then(language => {
            this.$q.lang.set(language.default)
          })
        } catch (e) {
          console.log(e)
        }
      },
      isSelected(value) {
        return value === this.$i18n.locale ? 'text-accent' : ''
      },
      getTitle() {
        return (this.langOptions.find(option => {
          return option.value === this.$i18n.locale
        })).label
      },
    },
  }
</script>

<style lang="sass">

</style>
