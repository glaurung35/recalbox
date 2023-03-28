<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.scraper.settings.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.scraper.settings.getNameFrom.title"
              :help="$t('settings.scraper.settings.getNameFrom.help')"
              :options="getNameFromOptions"
              :getter="scraper.getnamefrom"
              :setter="scraperStore.post"
              apiKey="getnamefrom"
              v-if="scraper.getnamefrom"
            />
          </div>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.scraper.screenscraper.medias.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.scraper.screenscraper.medias.media"
              :options="mediaOptions"
              :getter="scraper['screenscraper.media']"
              :setter="scraperStore.post"
              apiKey="screenscraper.media"
              v-if="scraper['screenscraper.media']"
            />
            <WrappedSelect
              label="settings.scraper.screenscraper.medias.thumbnail"
              :options="thumbnailOptions"
              :getter="scraper['screenscraper.thumbnail']"
              :setter="scraperStore.post"
              apiKey="screenscraper.thumbnail"
              v-if="scraper['screenscraper.thumbnail']"
            />
            <WrappedSelect
              label="settings.scraper.screenscraper.medias.video"
              :options="videoOptions"
              :getter="scraper['screenscraper.video']"
              :setter="scraperStore.post"
              apiKey="screenscraper.video"
              v-if="scraper['screenscraper.video']"
            />
            <WrappedToggle
              label="settings.scraper.screenscraper.medias.maps"
              :getter="scraper['screenscraper.maps']"
              :setter="scraperStore.post"
              apiKey="screenscraper.maps"
              v-if="scraper['screenscraper.maps']"
            />
            <WrappedToggle
              label="settings.scraper.screenscraper.medias.manual"
              :getter="scraper['screenscraper.manual']"
              :setter="scraperStore.post"
              apiKey="screenscraper.manual"
              v-if="scraper['screenscraper.manual']"
            />
            <WrappedToggle
              label="settings.scraper.screenscraper.p2k"
              :getter="scraper['screenscraper.p2k']"
              :setter="scraperStore.post"
              apiKey="screenscraper.p2k"
              v-if="scraper['screenscraper.p2k']"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-6">
      <FormFragmentContainer title="settings.scraper.screenscraper.auth.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedTextInput
              label="settings.scraper.screenscraper.auth.user"
              :getter="scraper['screenscraper.user']"
              :setter="scraperStore.post"
              apiKey="screenscraper.user"
              v-if="scraper['screenscraper.user']"
            />
            <WrappedTextInput
              label="settings.scraper.screenscraper.auth.password"
              :getter="scraper['screenscraper.password']"
              :setter="scraperStore.post"
              apiKey="screenscraper.password"
              v-if="scraper['screenscraper.password']"
              password
            />
          </div>
        </template>
      </FormFragmentContainer>
      <FormFragmentContainer title="settings.scraper.screenscraper.i18n.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedSelect
              label="settings.scraper.screenscraper.i18n.language"
              :options="languageOptions"
              :getter="scraper['screenscraper.language']"
              :setter="scraperStore.post"
              apiKey="screenscraper.language"
              v-if="scraper['screenscraper.language']"
            />
            <WrappedSelect
              label="settings.scraper.screenscraper.i18n.region"
              :options="regionOptions"
              :getter="scraper['screenscraper.region']"
              :setter="scraperStore.post"
              apiKey="screenscraper.region"
              v-if="scraper['screenscraper.region']"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import FormFragmentContainer from 'components/global/FormFragmentContainer.vue';
import { useScraperStore } from 'stores/scraper';
import { storeToRefs } from 'pinia';
import WrappedSelect from 'components/global/WrappedSelect.vue';
import WrappedTextInput from 'components/global/WrappedTextInput.vue';
import WrappedToggle from 'components/global/WrappedToggle.vue';

const scraperStore = useScraperStore();
scraperStore.fetch();
const {
  scraper,
  getNameFromOptions,
  languageOptions,
  regionOptions,
  mediaOptions,
  thumbnailOptions,
  videoOptions,
} = storeToRefs(scraperStore);
</script>
