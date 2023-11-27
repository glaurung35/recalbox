<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row systems">
    <div class="col col-xs-12 col-sm-12 col-md-12 table-container">
      <q-table
        grid
        card-container-class="card-container"
        card-class="bg-secondary text-white card"
        :rows="systemsList"
        :columns="columns"
        row-key="name"
        :filter="table.filter"
        hide-header
        square
        flat
        dense
        :pagination="table.pagination"
      >
        <template v-slot:top-right>
          <q-input
            :placeholder="$t('general.tables.searchLabel')"
            debounce="300"
            dense
            standout="bg-primary text-white"
            v-model="table.filter"
            class="search"
          >
            <template v-slot:prepend>
              <q-icon name="mdi-magnify"/>
            </template>
            <template v-slot:append v-if="table.filter !== ''">
              <q-icon
                name="mdi-close"
                @click="table.filter = ''"
                class="cursor-pointer"
              />
            </template>
          </q-input>
        </template>

        <template v-slot:item="props">
          <div
            @click="() => $router.push(
              { name: 'systems-system', params: { system: props.row.themeFolder }}
            )"
            class="q-pa-xs col-xs-12 col-sm-6 col-md-2 col-lg-2 grid-style-transition"
          >
            <q-card class="no-box-shadow no-border-radius card">
              <q-card-section>
                <div class="background"></div>
                <q-img
                  :src="api + '/systems/' + props.row.themeFolder + '/resource/eu/svg/logo'"
                  spinner-color="$light-blue"
                  :ratio="16/9"
                  fit="contain"
                  loading="lazy"
                />
                <div class="fullname">{{props.row.fullname}}</div>
              </q-card-section>
            </q-card>
          </div>
        </template>

        <template v-slot:no-data="{ icon, message, filter }">
          <div class="full-width row flex-center text-accent q-gutter-md q-ma-md">
            <q-icon name="mdi-emoticon-sad-outline" size="2em"/>
            <span>
              {{$t('general.tables.noDataMessage') + ' ' + message }}
            </span>
            <q-icon :name="filter ? 'mdi-layers-search-outline' : icon" size="2em"/>
          </div>
        </template>
      </q-table>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { useSystemsStore } from 'stores/systems';
import { computed, ref } from 'vue';
import { storeToRefs } from 'pinia';
import { apiUrl } from 'boot/axios';

const systemsStore = useSystemsStore();
systemsStore.fetch();
const { systemsList } = storeToRefs(systemsStore);

const api: string|undefined = apiUrl;
const columns = computed<object[]>(() => [
  {
    name: 'desc',
    required: true,
    label: '',
    align: 'left',
    field: (row: object) => row.fullname,
    sortable: true,
  },
]);

const table = ref<object>({
  filter: '',
  pagination: {
    rowsPerPage: 24,
  },
});
</script>

<style lang="sass">
.systems
  .table-container
    margin: 0 8px 74px
    background: white
    width: calc(100% - 16px)

    .card-container
      padding: 0 12px

      .card
        background: $rc-light-grey
        color: $light-blue
        text-align: center
        text-transform: uppercase
        cursor: pointer
        overflow: hidden
        border-radius: 5px!important

        .q-card__section
          padding: .5em 2.5em

          .q-img__image
            transition: filter .2s ease
            filter: saturate(0)
            opacity: 0.3

          .fullname, .background
            position: absolute
            left: 0
            right: 0
            transition: height .2s ease, top .35s ease

          .background
            top: 0
            height: 0
            background: #34495e6b

          .fullname
            top: -100%
            color: white
            font-weight: 700

        &:hover
          .q-card__section
            .q-img__image
              filter: saturate(1)
              opacity: 1

            .background
              height: 100%

            .fullname
              top: 0

  @media(max-width: 700px)
    .q-table__control, .search
      width: 100%
</style>
