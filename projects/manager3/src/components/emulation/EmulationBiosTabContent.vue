<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <q-table
        :columns="columns"
        :data="filteredBiosList"
        :filter="table.filter"
        :pagination.sync="table.pagination"
        binary-state-sort
        dense
        flat
        row-key="name"
        square
      >
        <template v-slot:top-left>
          <q-btn
            :color="filterButtons.redFilter ? 'negative' : 'light-blue'"
            @click="filterButtons.redFilter = !filterButtons.redFilter"
            flat
            icon="mdi-checkbox-marked-circle-outline"
            round
            size="md"
          >
            <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
              {{ $t('emulation.bios.tooltips.redFilter.label') }}
            </q-tooltip>
          </q-btn>
          <q-btn
            :color="filterButtons.yellowFilter ? 'orange-4' : 'light-blue'"
            @click="filterButtons.yellowFilter = !filterButtons.yellowFilter"
            flat
            icon="mdi-checkbox-marked-circle-outline"
            round
            size="md"
          >
            <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
              {{ $t('emulation.bios.tooltips.orangeFilter.label') }}
            </q-tooltip>
          </q-btn>
          <q-btn
            :color="filterButtons.greenFilter ? 'positive' : 'light-blue'"
            @click="filterButtons.greenFilter = !filterButtons.greenFilter"
            flat
            icon="mdi-checkbox-marked-circle-outline"
            round
            size="md"
          >
            <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
              {{ $t('emulation.bios.tooltips.greenFilter.label') }}
            </q-tooltip>
          </q-btn>
          <q-btn
            color="light-blue"
            @click="() => helpOpen = true"
            flat
            icon="mdi-help-circle-outline"
            round
            size="md"
          >
            <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
              {{ $t('emulation.bios.help.tooltip') }}
            </q-tooltip>
          </q-btn>
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
            <template v-slot:append>
              <q-icon @click="table.filter = ''" class="cursor-pointer" name="mdi-close" v-if="table.filter !== ''"/>
              <q-icon name="mdi-magnify"/>
            </template>
          </q-input>
          <q-btn class="addButton" color="accent" icon="mdi-plus-thick" outline @click="openUpload"/>
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
          </q-tr>
        </template>

        <template v-slot:body="props">
          <q-tr :props="props">
            <q-td
              :key="col.name"
              :props="props"
              v-for="col in props.cols"
            >
              <span v-if="col.name === 'system'">
                {{ col.value }}
              </span>

              <span v-else-if="col.name === 'bios'">
                <q-btn
                  :label="col.value"
                  @click="copyToClipboard(col.value)"
                  class="copy-to-clipboard"
                  color="primary"
                  dense
                  flat
                  size="md"
                />
              </span>

              <span class="md5" v-else-if="col.name === 'currentMd5'">
                {{ col.value !== '00000000000000000000000000000000' ? col.value : '-' }}
              </span>

              <span v-else-if="col.name === 'md5'">
                <q-btn
                  :label="col.value[0]"
                  @click="copyToClipboard(col.value)"
                  class="copy-to-clipboard md5"
                  color="primary"
                  dense
                  flat
                  size="md"
                />
                <q-btn
                  @click="md5Open = true, selectedMd5 = col.value"
                  class="md5"
                  color="primary"
                  dense
                  flat
                  icon="mdi-dots-horizontal"
                  round
                  v-if="col.value.length > 1"
                />
              </span>

              <span v-else-if="col.name === 'valid' && col.value === 'Green'">
                <q-icon color="positive" name="mdi-checkbox-marked-circle-outline" size="sm">
                  <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
                    {{ $t('emulation.bios.tooltips.green') }}
                  </q-tooltip>
                </q-icon>
              </span>
              <span v-else-if="col.name === 'valid' && col.value === 'Yellow'">
                <q-icon color="orange-4" name="mdi-checkbox-marked-circle-outline" size="sm">
                  <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
                    {{ $t('emulation.bios.tooltips.yellow') }}
                  </q-tooltip>
                </q-icon>
              </span>
              <span v-else-if="col.name === 'valid' && col.value === 'Red'">
                <q-icon color="negative" name="mdi-checkbox-marked-circle-outline" size="sm">
                  <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
                    {{ $t('emulation.bios.tooltips.red') }}
                  </q-tooltip>
                </q-icon>
              </span>
              <span v-else-if="col.name === 'delete'">
                <q-btn
                  color="accent"
                  dense
                  flat
                  icon="mdi-delete"
                  round
                  size="md"
                  @click="openConfirm(col.value)"
                >
                  <q-tooltip :offset="[10, 10]" content-class="bg-primary" content-style="font-size: 16px">
                    {{ $t('emulation.bios.tooltips.delete') }}
                  </q-tooltip>
                </q-btn>
              </span>
            </q-td>
          </q-tr>
        </template>

        <template v-slot:no-data="{ icon, message, filter }">
          <div class="full-width row flex-center text-accent q-gutter-md q-ma-md">
            <q-icon name="mdi-emoticon-sad-outline" size="2em"/>
            <span>
              {{ $t('general.tables.noDataMessage') + ' ' + message }}
            </span>
            <q-icon :name="filter ? 'mdi-layers-search-outline' : icon" size="2em"/>
          </div>
        </template>

      </q-table>

      <q-dialog transition-hide="flip-up" transition-show="flip-down" v-model="md5Open">
        <q-card class="bg-primary text-white">
          <q-toolbar>
            <q-toolbar-title>MD5s</q-toolbar-title>
            <q-btn @click="copyToClipboard(selectedMd5)" color="accent" dense flat icon="mdi-content-copy" round
                   size="md"/>
          </q-toolbar>

          <q-card-section class="q-pt-none">
            <div :key="md5.value" class="md5" v-for="md5 in selectedMd5">
              {{ md5 }}
            </div>
          </q-card-section>
        </q-card>
      </q-dialog>

      <q-dialog transition-hide="flip-up" transition-show="flip-down" v-model="helpOpen">
        <q-card class="bg-primary text-white">
          <q-toolbar>
            <q-toolbar-title>{{ $t('emulation.bios.help.title') }}</q-toolbar-title>
          </q-toolbar>

          <q-card-section class="q-pt-none">
            <div class="help-line row">
              <div class="col-1">
                <q-icon name="mdi-checkbox-marked-circle-outline" color="positive" size="sm"/>
              </div>
              <div class="col">BIOS présent + MD5 valide</div>
            </div>
            <div class="help-line row">
              <div class="col-1">
                <q-icon name="mdi-checkbox-marked-circle-outline" color="orange-4" size="sm"/>
              </div>
              <div class="col">
                BIOS présent + MD5 invalide mais MD5 match non obligatoire<br>
                <span>
                  Ça peut fonctionner, mais comme personne de chez nous ne l'a testé, on ne garantie rien
                </span>
              </div>
            </div>
            <div class="help-line row">
              <div class="col-1">
                <q-icon name="mdi-checkbox-marked-circle-outline" color="orange-4" size="sm"/>
              </div>
              <div class="col">
                BIOS absent mais optionnel<br>
                <span>
                  Ça peut fonctionner, tant qu'on utilise pas un jeu ou
                  une fonctionnalité qui requiert ce bios optionnel
                </span>
              </div>
            </div>
            <div class="row">
              <div class="col-1">
                <q-icon name="mdi-checkbox-marked-circle-outline" color="negative" size="sm"/>
              </div>
              <div class="col">BIOS absent</div>
            </div>
          </q-card-section>
        </q-card>
      </q-dialog>

    </div>
  </div>
</template>

<script>
import tools from '../../tools/index'

export default {
  name: 'EmulationBiosTabContent',
  methods: {
    openUpload() {
      this.$root.$emit('open-upload', 'bios')
    },
    copyToClipboard(content) {tools.copyToClipboard(content)},
    openConfirm(slug) {
      this.selectedSlug = slug
      this.$root.$emit(
        'confirm-open',
        this.remove,
        'mdi-alert-box-outline',
        this.$t('emulation.bios.dialogs.remove.text'),
      )
    },
    remove() {
      this.$store.dispatch('bios/remove', this.selectedSlug)
      this.selectedSlug = null
    },
  },
  created() {
    this.$store.dispatch('bios/get')
  },
  computed: {
    bios: {
      get: function () {
        return this.$store.getters['bios/list'] ? this.$store.getters['bios/list'] : []
      },
    },
    filteredBiosList: function () {
      let result = Object.values(this.bios)

      if (!this.filterButtons.redFilter) result = result.filter(bios => bios.lightStatus !== 'Red')
      if (!this.filterButtons.yellowFilter) result = result.filter(bios => bios.lightStatus !== 'Yellow')
      if (!this.filterButtons.greenFilter) result = result.filter(bios => bios.lightStatus !== 'Green')

      return result
    },
    columns: function () {
      return [
        {
          name: 'system',
          required: true,
          label: this.$t('emulation.bios.tableHeader.system'),
          align: 'left',
          field: 'system',
          sortable: true,
        },
        {
          name: 'bios',
          required: true,
          label: this.$t('emulation.bios.tableHeader.bios'),
          align: 'left',
          field: 'displayFileName',
          sortable: true,
        },
        {
          name: 'currentMd5',
          required: true,
          label: this.$t('emulation.bios.tableHeader.currentMd5'),
          align: 'left',
          field: 'currentMd5',
          sortable: true,
        },
        {
          name: 'md5',
          align: 'left',
          label: this.$t('emulation.bios.tableHeader.md5'),
          field: 'md5List',
          sortable: true,
        },
        {
          name: 'valid',
          align: 'center',
          label: this.$t('emulation.bios.tableHeader.valid'),
          field: 'lightStatus',
          sortable: true,
        },
        {
          name: 'delete',
          align: 'center',
          label: '',
          field: 'displayFileName',
          sortable: false,
        },
      ]
    },
  },
  data() {
    return {
      selectedSlug: null,
      table: {
        filter: '',
        pagination: {
          rowsPerPage: 10,
          descending: false,
          sortBy: 'system',
        },
      },
      md5Open: false,
      helpOpen: false,
      selectedMd5: null,
      filterButtons: {
        redFilter: true,
        yellowFilter: true,
        greenFilter: true,
      },
    }
  },
}
</script>

<style lang="sass">
.copy-to-clipboard
  font-weight: inherit
  text-transform: inherit

.q-table__card
  margin: 0 .5em 74px

  .addButton
    margin-left: 16px

  @media(max-width: 700px)
    .search
      flex: 5

    .addButton
      flex: 1
      margin-left: 10px

  .md5
    font-family: monospace
    font-size: 13px !important

.help-line
  margin-bottom: 1em

  span
    color: $dark-blue
</style>
