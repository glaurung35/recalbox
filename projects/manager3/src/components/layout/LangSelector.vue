<!--
@author Nicolas TESSIER aka Asthonishia
-->
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
              :src="'images/flags/' + langOption.value + '.png'"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t(langOption.label) }}</q-item-label>
          </q-item-section>
        </q-item>
      </q-list>
    </q-menu>
  </q-item>
</template>

<script lang="ts" setup>
import { Quasar, LocalStorage, useQuasar } from 'quasar';
import { useI18n } from 'vue-i18n';

const { locale } = useI18n({ useScope: 'global' });
const lang = useQuasar().lang.getLocale();
const langOptions = [
  { value: 'en-US', label: 'i18nMenu.english' },
  { value: 'fr', label: 'i18nMenu.french' },
];
function isSelected(value: string) {
  return value === locale.value ? 'text-accent' : '';
}
function getTitle() {
  return (langOptions.find(
    (option) => option.value === locale.value,
  ))?.label;
}
async function select(langValue: string) {
  try {
    const langIso = langValue === 'fr-fr' ? 'fr' : langValue;

    locale.value = langValue; // Update translation locale
    LocalStorage.set('locale', langValue); // Persist locale on LocalStorage to apply on refresh

    await import(
      /* webpackInclude: /(fr|en-US)\.js$/ */
      `quasar/lang/${langIso}`
    ).then((language) => {
      Quasar.lang.set(language.default); // Update Quasar components translation
    });
  } catch (error) {
    // eslint-disable-next-line no-console
    console.log(error);
  }
}
</script>
