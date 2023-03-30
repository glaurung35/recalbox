<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page class="background monitoring">
    <CPUChartContent :data="JSON.parse(JSON.stringify(areaChartData))"/>
    <div class="charts-container row">
      <div class="col cores-usage">
        <CoresChartContent :data="cores"/>
      </div>
      <div class="col disks-usage">
        <StoragesChartContent :data="storages"/>
      </div>
    </div>
  </q-page>
</template>

<script lang="ts" setup>
import { useMonitoringStore } from 'stores/monitoring';
import { storeToRefs } from 'pinia';
import { computed } from 'vue';
import CPUChartContent from 'components/monitoring/CPUChartContent.vue';
import CoresChartContent from 'components/monitoring/CoresChartContent.vue';
import StoragesChartContent from 'components/monitoring/StoragesChartContent.vue';

const monitoringStore = useMonitoringStore();
const { metrics, monitoring } = storeToRefs(monitoringStore);

monitoringStore.fetch();

const cores = computed<object[]>(() => [{
  data: metrics.value.cores,
}]);
const areaChartData = computed<object[]>(() => [
  { data: metrics.value.temperatures },
  { data: metrics.value.memory },
]);
const storages = computed(() => monitoring.value.storages);
</script>

<style lang="sass">
.monitoring
  position: relative

  &:before
    content: "\F076A"

  .charts-container
    position: relative
    padding: 16px 8px

    .col
      background: white
      margin: 0 8px 1em

  .cores-usage
    flex: 2
    min-height: 508px

  .disks-usage
    flex: 1
    padding: .5em

    .progress
      border: 1px solid rgba(240, 240, 240, 1)

      .q-linear-progress__model
        opacity: .5

      .progress-text
        height: 100%
        line-height: 28px
        font-size: .4em
        color: $primary

        span
          vertical-align: middle

      i
        font-size: .7em
        margin-left: .2em
        margin-right: .2em
</style>
