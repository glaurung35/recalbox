export interface Segment {
  id: string;
  start_time: string;
  end_time: string;
  title: string;
  canceled_until: null,
  category: {
    id: string;
    name: string;
  },
  is_recurring: boolean;
}

export interface TwitchScheduleResponse {
  data: {
    segments: Segment[],
    broadcaster_id: string;
    broadcaster_name: string;
    broadcaster_login: string;
    vacation: null
  },
  pagination: {
    cursor: string;
  }
}
