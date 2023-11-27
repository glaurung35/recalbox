<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <q-table
        :columns="columns"
        :rows="filteredBiosList"
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
            :color="filterButtons.redFilter ? 'negative' : 'light-blue'"
            @click="filterButtons.redFilter = !filterButtons.redFilter"
            flat
            icon="mdi-checkbox-marked-circle-outline"
            round
            size="md"
          >
            <q-tooltip
              class="bg-primary"
              :offset="[10, 10]"
              content-class="bg-primary"
              content-style="font-size: 16px"
            >
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
            <q-tooltip
              class="bg-primary"
              :offset="[10, 10]"
              content-class="bg-primary"
              content-style="font-size: 16px"
            >
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
            <q-tooltip
              class="bg-primary"
              :offset="[10, 10]"
              content-class="bg-primary"
              content-style="font-size: 16px"
            >
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
            <q-tooltip
              class="bg-primary"
              :offset="[10, 10]"
              content-class="bg-primary"
              content-style="font-size: 16px"
            >
              {{ $t('emulation.bios.help.tooltip') }}
            </q-tooltip>
          </q-btn>
        </template>

        <template v-slot:top-right>
          <q-btn
            class="refreshButton"
            color="accent"
            icon="mdi-refresh"
            outline
            @click="refresh"
          >
            <q-tooltip
              class="bg-primary"
              :offset="[10, 10]"
              content-class="bg-primary"
              content-style="font-size: 16px"
            >
              {{ $t('emulation.bios.tooltips.refresh.label') }}
            </q-tooltip>
          </q-btn>
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
<!--          <q-btn-->
<!--            class="addButton"-->
<!--            color="accent"-->
<!--            icon="mdi-plus-thick"-->
<!--            outline-->
<!--            @click="openUpload"-->
<!--          />-->
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
                <q-breadcrumbs gutter="xs">
                  <q-breadcrumbs-el
                    :key="pathNode"
                    v-for="pathNode in col.value.split('/')"
                  >
                    <q-btn
                      :label="pathNode"
                      @click="copy(pathNode)"
                      :class="getBiosClass(col, pathNode)"
                      color="primary"
                      dense
                      flat
                      size="md"
                      :icon="getBreadCrumbIcon(col, pathNode)"
                    />
                  </q-breadcrumbs-el>
                </q-breadcrumbs>
              </span>

              <span class="md5" v-else-if="col.name === 'currentMd5'">
                <q-btn
                  :label="col.value"
                  @click="copy(col.value)"
                  class="copy-to-clipboard"
                  color="primary"
                  dense
                  flat
                  size="md"
                  v-if="col.value !== '00000000000000000000000000000000'"
                />
                {{ col.value === '00000000000000000000000000000000' ? '-' : '' }}
              </span>

              <span v-else-if="col.name === 'md5'">
                <q-btn
                  :label="col.value[0]"
                  @click="copy(col.value)"
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
                  <q-tooltip
                    class="bg-primary"
                    :offset="[10, 10]"
                    content-class="bg-primary"
                    content-style="font-size: 16px"
                  >
                    {{ $t('emulation.bios.tooltips.green') }}
                  </q-tooltip>
                </q-icon>
              </span>
              <span v-else-if="col.name === 'valid' && col.value === 'Yellow'">
                <q-icon color="orange-4" name="mdi-checkbox-marked-circle-outline" size="sm">
                  <q-tooltip
                    class="bg-primary"
                    :offset="[10, 10]"
                    content-class="bg-primary"
                    content-style="font-size: 16px"
                  >
                    {{ $t('emulation.bios.tooltips.yellow') }}
                  </q-tooltip>
                </q-icon>
              </span>
              <span v-else-if="col.name === 'valid' && col.value === 'Red'">
                <q-icon color="negative" name="mdi-checkbox-marked-circle-outline" size="sm">
                  <q-tooltip
                    class="bg-primary"
                    :offset="[10, 10]"
                    content-class="bg-primary"
                    content-style="font-size: 16px"
                  >
                    {{ $t('emulation.bios.tooltips.red') }}
                  </q-tooltip>
                </q-icon>
              </span>
<!--              <span v-else-if="col.name === 'delete'">-->
<!--                <q-btn-->
<!--                  color="accent"-->
<!--                  dense-->
<!--                  flat-->
<!--                  icon="mdi-delete"-->
<!--                  round-->
<!--                  size="md"-->
<!--                  @click="openConfirm(col.value)"-->
<!--                >-->
<!--                  <q-tooltip-->
<!--                    class="bg-primary"-->
<!--                    :offset="[10, 10]"-->
<!--                    content-class="bg-primary"-->
<!--                    content-style="font-size: 16px"-->
<!--                  >-->
<!--                    {{ $t('emulation.bios.tooltips.delete') }}-->
<!--                  </q-tooltip>-->
<!--                </q-btn>-->
<!--              </span>-->
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

      <q-dialog transition-hide="slide-down" transition-show="slide-up" v-model="md5Open">
        <q-card class="bg-primary text-white">
          <q-toolbar>
            <q-toolbar-title>MD5s</q-toolbar-title>
            <q-btn
              @click="copy(selectedMd5)"
              color="accent"
              dense
              flat
              icon="mdi-content-copy"
              round
              size="md"
            />
          </q-toolbar>

          <q-card-section class="q-pt-none">
            <div :key="md5.value" class="md5" v-for="md5 in selectedMd5">
              {{ md5 }}
            </div>
          </q-card-section>
        </q-card>
      </q-dialog>

      <q-dialog transition-hide="flip-up" transition-show="flip-down" v-model="helpOpen">
        <q-card class="bg-primary text-white md5-dialog-card">
          <q-toolbar>
            <q-toolbar-title>{{ $t('emulation.bios.help.title') }}</q-toolbar-title>
          </q-toolbar>

          <q-card-section class="q-pt-none">
            <EmulationBiosTabHelpContent />
          </q-card-section>
        </q-card>
      </q-dialog>

    </div>
  </div>
</template>

<script lang="ts" setup>
import { useBiosStore } from 'stores/bios';
import { copy } from 'src/utils/copy';
import { storeToRefs } from 'pinia';
import {
  computed,
  ref,
  reactive,
} from 'vue';
import { useI18n } from 'vue-i18n';
import EmulationBiosTabHelpContent from 'components/emulation/EmulationBiosTabHelpContent.vue';

const { t } = useI18n({ useScope: 'global' });

const biosStore = useBiosStore();
biosStore.fetch();
const { biosList } = storeToRefs(biosStore);

const columns = computed<object[]>(() => [
  {
    name: 'system',
    required: true,
    label: t('emulation.bios.tableHeader.system'),
    align: 'left',
    field: 'system',
    sortable: true,
  },
  {
    name: 'bios',
    required: true,
    label: t('emulation.bios.tableHeader.bios'),
    align: 'left',
    field: 'displayFileName',
    sortable: true,
  },
  {
    name: 'currentMd5',
    required: true,
    label: t('emulation.bios.tableHeader.currentMd5'),
    align: 'left',
    field: 'currentMd5',
    sortable: true,
  },
  {
    name: 'md5',
    align: 'left',
    label: t('emulation.bios.tableHeader.md5'),
    field: 'md5List',
    sortable: true,
  },
  {
    name: 'valid',
    align: 'center',
    label: t('emulation.bios.tableHeader.valid'),
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
]);

// const selectedSlug = ref<string>('');
const md5Open = ref<boolean>(false);
const helpOpen = ref<boolean>(false);
const selectedMd5 = ref<string>('');
const filterButtons = reactive<object>({
  redFilter: true,
  yellowFilter: true,
  greenFilter: true,
});
const table = ref<object>({
  filter: '',
  pagination: {
    rowsPerPage: 15,
    descending: false,
    sortBy: 'system',
  },
});

const filteredBiosList = computed(() => {
  let list = biosList.value;

  if (!filterButtons.redFilter) list = list.filter((filteredBios) => filteredBios.lightStatus !== 'Red');
  if (!filterButtons.yellowFilter) list = list.filter((filteredBios) => filteredBios.lightStatus !== 'Yellow');
  if (!filterButtons.greenFilter) list = list.filter((filteredBios) => filteredBios.lightStatus !== 'Green');

  return list;
});

function refresh() {
  biosStore.fetch();
}

function isBios(column:object, pathNode:string) {
  return column.value.split('/').slice(-1).pop() === pathNode;
}

function getBiosClass(column:object, pathNode:string) {
  return isBios(column, pathNode) ? 'path bios copy-to-clipboard' : 'path copy-to-clipboard';
}

function getBreadCrumbIcon(column:object, pathNode:string) {
  return isBios(column, pathNode) ? 'mdi-chip' : 'mdi-folder-open-outline';
}
//   methods: {
//     openUpload() {
//       this.$root.$emit('open-upload', 'bios');
//     },
//     openConfirm(slug) {
//       this.selectedSlug = slug
//       this.$root.$emit(
//         'confirm-open',
//         this.remove,
//         'mdi-alert-box-outline',
//         this.$t('emulation.bios.dialogs.remove.text'),
//       )
//     },
//     remove() {
//       this.$store.dispatch('bios/remove', this.selectedSlug)
//       this.selectedSlug = ''
//     },
//   },
</script>

<style lang="sass">
.copy-to-clipboard
  font-weight: inherit
  text-transform: inherit

.path
  .q-icon
    color: $light-blue
  &.bios
    .q-icon
      color: $accent

.q-table__card
  margin: 0 .5em 74px

  .addButton
    margin-left: 16px

  .refreshButton
    margin-right: 16px

  @media(max-width: 700px)
    .search
      flex: 5

    .addButton
      flex: 1
      margin-left: 10px

  .md5
    font-family: monospace
    font-size: 13px !important

.md5-dialog-card
  border-left: 6px solid $accent
</style>
