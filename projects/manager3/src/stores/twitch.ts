/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { TWITCH } from 'src/router/api.routes';
import { runSequentialPromises } from 'quasar';
import { HttpClientProviderStore } from 'stores/plugins/httpClientProviderStorePlugin';
import { Segment } from 'stores/types/twitch';

const perPage = 7;
const broadcasterId = '115060112';

export interface TwitchStoreState extends HttpClientProviderStore {
  schedule: Segment[];
  loadingSchedule: boolean;
}

export const useTwitchStore = defineStore('twitch', {
  state: () => ({
    schedule: {},
    loadingSchedule: false,
  } as TwitchStoreState),

  actions: {
    getSchedule() {
      this.loadingSchedule = true;
      runSequentialPromises({
        auth: () => this._httpClientProvider.post(TWITCH.auth, {
          client_id: process.env.TWITCH_CLIENT_ID,
          client_secret: process.env.TWITCH_CLIENT_SECRET,
          grant_type: 'client_credentials',
        }),
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        schedule: (resultAggregator) => this._httpClientProvider.create({
          headers: {
            // eslint-disable-next-line @typescript-eslint/ban-ts-comment
            // @ts-ignore
            Authorization: `Bearer ${resultAggregator.auth.value.data.access_token}`,
            'Client-Id': process.env.TWITCH_CLIENT_ID,
          },
        }).get(TWITCH.schedule, { params: { broadcaster_id: broadcasterId, first: perPage } }),
      }).then((resultAggregator) => {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        this.schedule = resultAggregator.schedule.value.data.data.segments;
        this.loadingSchedule = false;
      }).catch((errResult) => {
        // eslint-disable-next-line no-console
        console.error(`Error encountered on job (${errResult.key}):`);
        // eslint-disable-next-line no-console
        console.error(errResult.reason);
        // eslint-disable-next-line no-console
        console.log('Managed to get these results before this error:');
        // eslint-disable-next-line no-console
        console.log(errResult.resultAggregator);
      });
    },
  },
});
