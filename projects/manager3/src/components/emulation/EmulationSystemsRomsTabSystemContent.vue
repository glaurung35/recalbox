<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row system">
    <div class="col col-xs-12 col-sm-12 col-md-12 table-container">
      <q-table
        :columns="columns"
        :rows="roms"
        :filter="table.filter"
        :pagination="table.pagination"
        binary-state-sort
        dense
        flat
        row-key="name"
        square
      >
        <template v-slot:top-left>
          <q-btn
            color="accent"
            square
            icon="mdi-chevron-left"
            size="md"
            flat
            @click="() => router.go(-1)"
            style="margin-right: 1em;"
          >
            {{$t('general.return')}}
          </q-btn>
          <q-img
            class="logo"
            :src="logoUrl"
            spinner-color="white"
          />
        </template>
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
<!--          <q-btn class="q-ml-md" color="accent" icon="mdi-plus-thick" outline/>-->
        </template>

        <template v-slot:header="props">
          <q-tr :props="props">
            <q-th
              :key="col.name"
              :props="props"
              v-for="col in props.cols"
            >
              {{ col.label }}
            </q-th>
<!--            <q-th auto-width/>-->
<!--            <q-th auto-width/>-->
          </q-tr>
        </template>

        <template v-slot:body="props">
          <q-tr :props="props">
            <q-td
              :key="col.name"
              :props="props"
              v-for="col in props.cols"
            >
              <span v-if="col.name === 'rating'">
                <q-rating
                  :model-value="col.value * 5"
                  class=“no-shadow”
                  color="accent"
                  readonly
                  size="1em"
                  icon="stars"
                />
              </span>
              <span v-else-if="col.name === 'genre'">
                {{ (col.value).replaceAll(',', ', ') }}
              </span>
              <span v-else>
                {{ col.value }}
              </span>
            </q-td>
<!--            <q-td auto-width>-->
<!--              <q-btn color="primary" dense flat icon="mdi-pencil" round size="md"/>-->
<!--            </q-td>-->
<!--            <q-td auto-width>-->
<!--              <q-btn-->
<!--                color="negative"-->
<!--                dense-->
<!--                flat-->
<!--                icon="mdi-delete"-->
<!--                round size="md"-->
<!--              />-->
<!--            </q-td>-->
          </q-tr>
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
import { useRomsStore } from 'stores/roms';
import { useSystemsStore } from 'stores/systems';
import { computed, ref } from 'vue';
import { useRouter, useRoute } from 'vue-router';
import { apiUrl } from 'boot/axios';
import { storeToRefs } from 'pinia';

const systemsStore = useSystemsStore();
const { systems } = systemsStore;
const router = useRouter();
const { system } = useRoute().params;

const logoUrl = computed(() => {
  const filteredSystems = systems.systems.filter((s) => s.name === useRoute().params.system);
  return `${apiUrl}/systems/${filteredSystems[0].themeFolder}/resource/eu/svg/logo`;
});

const romsStore = useRomsStore();
romsStore.fetchBySystem(system as string);

const columns = [
  {
    name: 'nom', required: true, align: 'left', label: 'Nom', field: 'name', sortable: true,
  },
  {
    name: 'editeur', align: 'left', label: 'Éditeur', field: 'publisher', sortable: true,
  },
  {
    name: 'developpeur', align: 'left', label: 'Développeur', field: 'developer', sortable: true,
  },
  {
    name: 'genre', align: 'left', label: 'Genre', field: 'genre', sortable: true,
  },
  {
    name: 'joueurs', align: 'center', label: 'Joueurs', field: 'players', sortable: true,
  },
  {
    name: 'rating', align: 'center', label: 'Note', field: 'rating', sortable: true,
  },
];
const table = ref({
  filter: '',
  pagination: {
    rowsPerPage: 20,
    descending: false,
    sortBy: 'nom',
  },
});

const { roms } = storeToRefs(romsStore);
</script>

<style lang="sass">
.system
  .table-container
    margin: 0 8px 74px
    width: calc(100% - 16px)

    .q-table__top
      margin-bottom: 10px

    .logo
      width: 100px
      max-height: 50px

    @media(max-width: 700px)
      .search
        flex: 5
</style>
