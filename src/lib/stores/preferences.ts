import { writable } from 'svelte/store'

export const dataCharts = writable<{ x: string; y: number }[]>([]);
