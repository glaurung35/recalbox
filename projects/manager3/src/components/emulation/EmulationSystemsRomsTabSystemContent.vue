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
            alt="Sammy Atomiswave"
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
            <q-th auto-width/>
            <q-th auto-width/>
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
                  v-model="col.value"
                  class=“no-shadow”
                  color="accent"
                  readonly
                  size="1em"
                  icon="stars"
                />
              </span>
              <span v-else>
                {{ col.value }}
              </span>
            </q-td>
<!--            <q-td auto-width>-->
<!--              <q-btn color="primary" dense flat icon="mdi-pencil" round size="md"/>-->
<!--            </q-td>-->
            <q-td auto-width>
              <q-btn
                color="negative"
                dense
                flat
                icon="mdi-delete"
                round size="md"
              />
            </q-td>
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
import { date } from 'quasar';
import { ref } from 'vue';
import { useRouter, useRoute } from 'vue-router';
import { apiUrl } from 'boot/axios';

const router = useRouter();
const logoUrl = `${apiUrl}/systems/${useRoute().params.system}/resource/eu/svg/logo`;

const columns: Array<object> = [
  {
    name: 'nom', required: true, align: 'left', label: 'Nom', field: 'nom', sortable: true,
  },
  {
    name: 'editeur', align: 'left', label: 'Éditeur', field: 'editeur', sortable: true,
  },
  {
    name: 'developpeur', align: 'left', label: 'Développeur', field: 'developpeur', sortable: true,
  },
  {
    name: 'genre', align: 'left', label: 'Genre', field: 'genre', sortable: true,
  },
  {
    name: 'dateSortie',
    align: 'left',
    label: 'Date de sortie',
    field: 'dateSortie',
    sortable: true,
    format: (val: string) => date.formatDate(date.extractDate(val, 'YYYY-MM-DD'), 'DD/MM/YYYY'),
  },
  {
    name: 'joueurs', align: 'center', label: 'Joueurs', field: 'joueurs', sortable: true,
  },
  {
    name: 'rating', align: 'center', label: 'Note', field: 'rating', sortable: true,
  },
];
const table = ref<object>({
  filter: '',
  pagination: {
    rowsPerPage: 20,
    descending: false,
    sortBy: 'nom',
  },
});

const roms: Array<object> = [
  {
    nom: 'Pokémon Puzzle Challenge',
    editeur: 'Nintendo',
    developpeur: 'Nintendo',
    genre: '',
    dateSortie: '',
    joueurs: '2',
    rating: 4,
  },
  {
    nom: 'Breakers Revenge',
    editeur: 'SNK',
    developpeur: 'Visco',
    genre: 'Combat-Combat / Versus',
    dateSortie: '1998-07-03',
    joueurs: '2',
    rating: 4,
  },
  {
    nom: 'SNK Gals Fighters',
    editeur: 'SNK',
    developpeur: 'SNK',
    genre: 'Action',
    dateSortie: '2000-01-01',
    joueurs: '1-2',
    rating: 4,
  },
  {
    nom: 'Bionic Commando: Elite Forces',
    editeur: 'Nintendo',
    developpeur: 'Nintendo',
    genre: 'Combat-Action',
    dateSortie: '2000-01-01',
    joueurs: '2',
    rating: 4,
  },
  {
    nom: 'Bikkuriman 2000 Viva!',
    editeur: 'Sega',
    developpeur: 'Sega',
    genre: 'Action',
    dateSortie: '2000-03-16',
    joueurs: '1',
    rating: 4,
  },
  {
    nom: 'Bahamut Lagoon',
    editeur: 'Square',
    developpeur: 'Square',
    genre: 'Jeu de rôles-Stratégie',
    dateSortie: '1996-02-09',
    joueurs: '2',
    rating: 4,
  },
  {
    nom: 'Earthworm Jim',
    editeur: 'Virgin',
    developpeur: 'Shiny Entertainment',
    genre: 'Plateforme-Action-Plateforme / Run Jump',
    dateSortie: '1994-10-01',
    joueurs: '1',
    rating: 5,
  },
  {
    nom: 'Sonic The Hedgehog 2',
    editeur: 'Sega',
    developpeur: 'Sega',
    genre: 'Plateforme-Action',
    dateSortie: '1992-01-01',
    joueurs: '2',
    rating: 4,
  },
  {
    nom: 'Hellfire',
    editeur: 'Sega',
    developpeur: 'Toaplan',
    genre: 'Action-Combat',
    dateSortie: '1990-11-01',
    joueurs: '2',
    rating: 2,
  },
  {
    nom: 'Best of the Best : Championship Karate',
    editeur: 'Electro Brain Corp.',
    developpeur: 'Loriciels',
    genre: 'Sport-Combat',
    dateSortie: '1993-07-01',
    joueurs: '2',
    rating: 4,
  },
];
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
